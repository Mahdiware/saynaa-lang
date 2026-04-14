#!/usr/bin/env python3

import argparse
import html
import json
import math
import platform
import re
import subprocess
import sys
import tempfile
from datetime import datetime, timezone
from pathlib import Path
from typing import Dict, List, Optional, Tuple


class CompareError(Exception):
    pass


def parse_args() -> Tuple[argparse.Namespace, List[Dict[str, object]]]:
    repo_root = Path(__file__).resolve().parents[1]
    default_html = repo_root / "tests" / "benchmark" / "results" / "compare.html"
    default_json = repo_root / "tests" / "benchmark" / "results" / "compare.json"

    parser = argparse.ArgumentParser(
        description="Compare multiple Saynaa binaries and generate an HTML report"
    )
    parser.add_argument("--warmup", type=int, default=2, help="Warmup runs per case")
    parser.add_argument("--iterations", type=int, default=6, help="Measured runs per case")
    parser.add_argument("--timeout", type=float, default=30.0, help="Timeout per run in seconds")
    parser.add_argument("--phase", action="append", default=[], help="Run only selected phase(s)")
    parser.add_argument(
        "--case",
        action="append",
        default=[],
        help="Run only selected case id(s) or script name(s), e.g. runtime_function_call.sa",
    )
    parser.add_argument(
        "--title",
        default="Saynaa Multi-App Benchmark Comparison",
        help="Report title",
    )
    parser.add_argument(
        "--html-out",
        default=str(default_html),
        help="Output HTML report path",
    )
    parser.add_argument(
        "--json-out",
        default=str(default_json),
        help="Output JSON comparison path",
    )

    args, unknown = parser.parse_known_args()

    app_paths: Dict[int, Path] = {}
    app_names: Dict[int, str] = {}
    i = 0
    while i < len(unknown):
        token = unknown[i]
        app_match = re.fullmatch(r"--app(\d+)", token)
        name_match = re.fullmatch(r"--name(\d+)", token)
        if not app_match and not name_match:
            parser.error(f"Unknown argument: {token}")

        if i + 1 >= len(unknown):
            parser.error(f"{token} requires a path argument")

        value = unknown[i + 1]
        if app_match:
            app_index = int(app_match.group(1))
            app_paths[app_index] = Path(value).expanduser().resolve()
        elif name_match:
            app_index = int(name_match.group(1))
            app_names[app_index] = value.strip()

        i += 2

    if not app_paths:
        parser.error("Provide at least two apps using --app1 <path> --app2 <path>")

    unknown_named_indexes = sorted(set(app_names) - set(app_paths))
    if unknown_named_indexes:
        parser.error(
            "Found --nameN without matching --appN for index(es): "
            + ", ".join(str(x) for x in unknown_named_indexes)
        )

    app_entries = []
    for app_index in sorted(app_paths):
        label = app_names.get(app_index) or f"app{app_index}"
        app_entries.append(
            {
                "index": app_index,
                "path": app_paths[app_index],
                "label": label,
            }
        )

    if len(app_entries) < 2:
        parser.error("At least two apps are required for comparison")

    if args.warmup < 0:
        parser.error("--warmup must be >= 0")
    if args.iterations <= 0:
        parser.error("--iterations must be > 0")
    if args.timeout <= 0:
        parser.error("--timeout must be > 0")

    return args, app_entries


def run_single_benchmark(
    repo_root: Path,
    runner_path: Path,
    app_index: int,
    app_label: str,
    app_path: Path,
    args: argparse.Namespace,
) -> Dict:
    if not app_path.exists():
        raise CompareError(f"App {app_index} not found: {app_path}")

    with tempfile.TemporaryDirectory(prefix=f"saynaa-compare-app{app_index}-") as tmp_dir_name:
        tmp_dir = Path(tmp_dir_name)
        single_out = tmp_dir / f"app{app_index}.json"

        cmd = [
            sys.executable,
            str(runner_path),
            "--app",
            str(app_path),
            "--warmup",
            str(args.warmup),
            "--iterations",
            str(args.iterations),
            "--timeout",
            str(args.timeout),
            "--json-out",
            str(single_out),
        ]

        for phase in args.phase:
            cmd.extend(["--phase", phase])
        for case_id in args.case:
            cmd.extend(["--case", case_id])

        print(f"Running app{app_index}: {app_path}")
        proc = subprocess.run(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
        if proc.returncode != 0:
            detail = proc.stderr.strip() or proc.stdout.strip()
            raise CompareError(
                f"Benchmark run failed for app{app_index} ({app_path}) with code {proc.returncode}\n{detail}"
            )

        try:
            single_report = json.loads(single_out.read_text(encoding="utf-8"))
        except (FileNotFoundError, json.JSONDecodeError) as exc:
            raise CompareError(f"Could not read benchmark output for app{app_index}: {exc}") from exc

        return {
            "index": app_index,
            "path": str(app_path),
            "label": app_label,
            "stdout": proc.stdout,
            "report": single_report,
        }


def get_case_order(app_reports: List[Dict]) -> List[str]:
    ordered_ids: List[str] = []
    seen = set()
    for app in app_reports:
        for row in app["report"].get("results", []):
            case_id = row.get("id")
            if isinstance(case_id, str) and case_id not in seen:
                seen.add(case_id)
                ordered_ids.append(case_id)
    return ordered_ids


def find_case_meta(app_reports: List[Dict], case_id: str) -> Dict:
    for app in app_reports:
        for row in app["report"].get("results", []):
            if row.get("id") == case_id:
                return {
                    "phase": row.get("phase", "unknown"),
                    "description": row.get("description", ""),
                    "mode": row.get("mode", ""),
                    "case_file": row.get("case_file", row.get("id", "")),
                    "script": row.get("script", ""),
                }
    return {
        "phase": "unknown",
        "description": "",
        "mode": "",
        "case_file": case_id,
        "script": "",
    }


def get_case_row_for_app(app_report: Dict, case_id: str) -> Optional[Dict]:
    for row in app_report["report"].get("results", []):
        if row.get("id") == case_id:
            return row
    return None


def geometric_mean(values: List[float]) -> Optional[float]:
    valid = [v for v in values if v > 0]
    if not valid:
        return None
    return math.exp(sum(math.log(v) for v in valid) / len(valid))


def build_comparison(app_reports: List[Dict], args: argparse.Namespace) -> Dict:
    case_order = get_case_order(app_reports)
    if not case_order:
        raise CompareError("No benchmark results found to compare")

    reference = app_reports[0]

    app_summary: Dict[int, Dict] = {
        app["index"]: {
            "label": app["label"],
            "path": app["path"],
            "wins": 0,
            "cases": 0,
            "median_values": [],
            "ratio_vs_reference": [],
        }
        for app in app_reports
    }

    case_rows = []
    for case_id in case_order:
        case_meta = find_case_meta(app_reports, case_id)
        app_cells = []
        medians = []

        ref_row = get_case_row_for_app(reference, case_id)
        ref_median = ref_row.get("median_ms") if ref_row else None

        for app in app_reports:
            app_row = get_case_row_for_app(app, case_id)
            median_ms = app_row.get("median_ms") if app_row else None
            p95_ms = app_row.get("p95_ms") if app_row else None
            ops_per_sec = app_row.get("ops_per_sec") if app_row else None

            app_cells.append(
                {
                    "app_index": app["index"],
                    "label": app["label"],
                    "median_ms": median_ms,
                    "p95_ms": p95_ms,
                    "ops_per_sec": ops_per_sec,
                }
            )

            if isinstance(median_ms, (int, float)) and median_ms > 0:
                medians.append(float(median_ms))
                app_summary[app["index"]]["median_values"].append(float(median_ms))
                app_summary[app["index"]]["cases"] += 1

                if isinstance(ref_median, (int, float)) and ref_median > 0:
                    app_summary[app["index"]]["ratio_vs_reference"].append(
                        float(median_ms) / float(ref_median)
                    )

        best_median = min(medians) if medians else None

        winners = []
        if best_median is not None:
            for cell in app_cells:
                median_ms = cell["median_ms"]
                if isinstance(median_ms, (int, float)) and abs(median_ms - best_median) < 1e-9:
                    winners.append(cell["app_index"])
                    app_summary[cell["app_index"]]["wins"] += 1

        for cell in app_cells:
            median_ms = cell["median_ms"]
            if isinstance(median_ms, (int, float)) and best_median and best_median > 0:
                cell["delta_vs_best_percent"] = ((median_ms - best_median) / best_median) * 100.0
                cell["ratio_vs_best"] = median_ms / best_median
            else:
                cell["delta_vs_best_percent"] = None
                cell["ratio_vs_best"] = None

        case_rows.append(
            {
                "id": case_id,
                "case_file": case_meta["case_file"],
                "script": case_meta["script"],
                "phase": case_meta["phase"],
                "description": case_meta["description"],
                "mode": case_meta["mode"],
                "best_median_ms": best_median,
                "winners": winners,
                "apps": app_cells,
            }
        )

    summary_rows = []
    for app in app_reports:
        index = app["index"]
        stats = app_summary[index]
        gm_median = geometric_mean(stats["median_values"])
        gm_ratio_ref = geometric_mean(stats["ratio_vs_reference"])
        summary_rows.append(
            {
                "app_index": index,
                "label": stats["label"],
                "path": stats["path"],
                "wins": stats["wins"],
                "cases": stats["cases"],
                "geomean_median_ms": gm_median,
                "geomean_ratio_vs_reference": gm_ratio_ref,
                "geomean_delta_vs_reference_percent": (
                    (gm_ratio_ref - 1.0) * 100.0 if gm_ratio_ref is not None else None
                ),
            }
        )

    summary_rows.sort(
        key=lambda row: (
            -(row["wins"] or 0),
            row["geomean_median_ms"] if row["geomean_median_ms"] is not None else float("inf"),
        )
    )

    return {
        "generated_at": datetime.now(timezone.utc).isoformat(),
        "title": args.title,
        "system": {
            "platform": platform.platform(),
            "machine": platform.machine(),
            "python": platform.python_version(),
        },
        "config": {
            "warmup": args.warmup,
            "iterations": args.iterations,
            "timeout_seconds": args.timeout,
            "selected_phases": args.phase,
            "selected_cases": args.case,
            "reference_app": reference["label"],
        },
        "apps": [
            {
                "app_index": app["index"],
                "label": app["label"],
                "path": app["path"],
            }
            for app in app_reports
        ],
        "summary": summary_rows,
        "cases": case_rows,
    }


def format_ms(value: Optional[float]) -> str:
    if value is None:
        return "-"
    return f"{value:.3f} ms"


def format_ops(value: Optional[float]) -> str:
    if value is None:
        return "-"
    return f"{value:,.0f} ops/s"


def format_pct(value: Optional[float]) -> str:
    if value is None:
        return "-"
    return f"{value:+.2f}%"


def render_html(report: Dict) -> str:
    title = html.escape(report["title"])
    generated_at = html.escape(report["generated_at"])
    config = report["config"]
    apps = report["apps"]
    summary = report["summary"]
    cases = report["cases"]

    phase_set = sorted({row["phase"] for row in cases})

    app_headers = "".join(
        f"<th>{html.escape(app['label'])}<br><small>{html.escape(app['path'])}</small></th>"
        for app in apps
    )

    summary_cards = []
    for rank, row in enumerate(summary, start=1):
        performance = row.get("geomean_delta_vs_reference_percent")
        delta_class = "better" if performance is not None and performance < 0 else "worse"
        if row["label"] == report["config"]["reference_app"]:
            delta_text = "Reference"
            delta_class = "neutral"
        else:
            delta_text = f"{format_pct(performance)} vs {report['config']['reference_app']}"

        if rank == 1:
            app_status_class = "card-rank-1"
        elif rank == 2:
            app_status_class = "card-rank-2"
        else:
            app_status_class = "card-rank-n"
        app_status_text = f"RANK {rank}"

        summary_cards.append(
            "".join(
                [
                    '<article class="card">',
                    f'<div class="rank">#{rank}</div>',
                    f"<h3>{html.escape(row['label'])}</h3>",
                    f'<span class="card-badge {app_status_class}">{app_status_text}</span>',
                    f"<p class=\"path\">{html.escape(row['path'])}</p>",
                    '<div class="stats">',
                    f"<div><span>Wins</span><strong>{row['wins']}</strong></div>",
                    f"<div><span>Cases</span><strong>{row['cases']}</strong></div>",
                    f"<div><span>GeoMean</span><strong>{format_ms(row['geomean_median_ms'])}</strong></div>",
                    "</div>",
                    f'<p class="delta {delta_class}">{html.escape(delta_text)}</p>',
                    "</article>",
                ]
            )
        )

    case_rows_html = []
    for row in cases:
        cells = []

        ranked_medians = sorted(
            {
                float(cell["median_ms"])
                for cell in row["apps"]
                if isinstance(cell.get("median_ms"), (int, float))
            }
        )
        rank_by_median = {
            median: idx for idx, median in enumerate(ranked_medians, start=1)
        }

        for cell in row["apps"]:
            delta_pct = cell.get("delta_vs_best_percent")
            ratio_vs_best = cell.get("ratio_vs_best")
            width_pct = 0.0
            if isinstance(ratio_vs_best, (int, float)) and ratio_vs_best > 0:
                width_pct = min(100.0, max(8.0, 100.0 / ratio_vs_best))

            median_ms = cell.get("median_ms")
            rank_number = None
            if isinstance(median_ms, (int, float)):
                rank_number = rank_by_median.get(float(median_ms))

            winner_class = "winner" if rank_number == 1 else ""
            if rank_number is None:
                status_class = "status-na"
                status_text = "RANK -"
            elif rank_number == 1:
                status_class = "status-rank-1"
                status_text = "RANK 1"
            elif rank_number == 2:
                status_class = "status-rank-2"
                status_text = "RANK 2"
            else:
                status_class = "status-rank-n"
                status_text = f"RANK {rank_number}"

            cells.append(
                "".join(
                    [
                        f'<td class="metric {winner_class}">',
                        f'<span class="status-pill {status_class}">{status_text}</span>',
                        f"<div class=\"median\">{format_ms(cell['median_ms'])}</div>",
                        f"<div class=\"bar\"><span style=\"width:{width_pct:.2f}%\"></span></div>",
                        f"<div class=\"delta\">{format_pct(delta_pct)} vs best</div>",
                        f"<div class=\"ops\">{format_ops(cell['ops_per_sec'])}</div>",
                        "</td>",
                    ]
                )
            )

        case_rows_html.append(
            "".join(
                [
                    f'<tr data-phase="{html.escape(row["phase"])}">',
                    '<td class="case-meta">',
                    f"<div class=\"case-id\">{html.escape(row['case_file'])}</div>",
                    f"<div class=\"case-alias\">id: {html.escape(row['id'])}</div>",
                    f"<div class=\"case-script\">{html.escape(row['script'])}</div>",
                    f"<div class=\"case-desc\">{html.escape(row['description'])}</div>",
                    f"<div class=\"phase-tag\">{html.escape(row['phase'])}</div>",
                    "</td>",
                    "".join(cells),
                    "</tr>",
                ]
            )
        )

    phase_buttons = [
        '<button class="phase-btn active" data-phase="all">All phases</button>'
    ]
    for phase in phase_set:
        phase_buttons.append(
            f'<button class="phase-btn" data-phase="{html.escape(phase)}">{html.escape(phase)}</button>'
        )

    return f"""<!doctype html>
<html lang=\"en\">
<head>
  <meta charset=\"utf-8\" />
  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\" />
  <title>{title}</title>
  <style>
    :root {{
      --bg-1: #f6f9f3;
      --bg-2: #e6f0e5;
      --ink: #0f1720;
      --muted: #4d5b66;
      --card: #ffffffd9;
      --line: #c9d6d0;
      --primary: #0f766e;
      --accent: #c2410c;
      --good: #0f766e;
      --bad: #b42318;
      --neutral: #475467;
      --shadow: 0 20px 40px rgba(15, 23, 32, 0.08);
    }}

    * {{ box-sizing: border-box; }}

    body {{
      margin: 0;
      min-height: 100vh;
      color: var(--ink);
      font-family: "Space Grotesk", "Manrope", "Segoe UI", sans-serif;
      background:
        radial-gradient(1300px 600px at 10% -10%, #dbefe7 0%, transparent 70%),
        radial-gradient(1200px 600px at 100% 0%, #f2dfcf 0%, transparent 65%),
        linear-gradient(160deg, var(--bg-1), var(--bg-2));
    }}

    .container {{
      width: min(1240px, calc(100vw - 2rem));
      margin: 1.25rem auto 2rem;
      display: grid;
      gap: 1rem;
    }}

    .hero {{
      background: var(--card);
      border: 1px solid var(--line);
      box-shadow: var(--shadow);
      border-radius: 18px;
      padding: 1rem 1.1rem;
      display: grid;
      gap: 0.75rem;
    }}

    .hero h1 {{
      margin: 0;
      font-size: clamp(1.25rem, 2.5vw, 1.9rem);
      letter-spacing: 0.02em;
    }}

    .meta {{
      display: flex;
      flex-wrap: wrap;
      gap: 0.5rem;
    }}

    .chip {{
      border: 1px solid #9db9ad;
      color: #17463e;
      background: #f1f8f3;
      border-radius: 999px;
      padding: 0.26rem 0.65rem;
      font-size: 0.82rem;
    }}

    .card-grid {{
      display: grid;
      grid-template-columns: repeat(auto-fit, minmax(220px, 1fr));
      gap: 0.85rem;
    }}

    .card {{
      position: relative;
      background: var(--card);
      border: 1px solid var(--line);
      border-radius: 16px;
      box-shadow: var(--shadow);
      padding: 0.9rem;
      display: grid;
      gap: 0.6rem;
    }}

    .rank {{
      position: absolute;
      top: 0.6rem;
      right: 0.65rem;
      color: #2f4137;
      background: #eef6f0;
      border: 1px solid #c9ddd1;
      border-radius: 999px;
      padding: 0.08rem 0.45rem;
      font-size: 0.76rem;
    }}

    .card h3 {{
      margin: 0;
      font-size: 1.08rem;
      color: #153727;
    }}

    .card .path {{
      margin: 0;
      font-size: 0.76rem;
      color: var(--muted);
      word-break: break-word;
    }}

    .card-badge {{
      display: inline-block;
      width: fit-content;
      font-size: 0.68rem;
      font-weight: 700;
      border-radius: 999px;
      padding: 0.08rem 0.48rem;
      border: 1px solid transparent;
      letter-spacing: 0.02em;
      text-transform: uppercase;
    }}

    .card-badge.card-rank-1 {{
      color: #1e5f3b;
      background: #e6f6ee;
      border-color: #bfe4d0;
    }}

    .card-badge.card-rank-2 {{
      color: #8f2930;
      background: #fcecef;
      border-color: #f2c6cd;
    }}

    .card-badge.card-rank-n {{
      color: #8f2930;
      background: #fcecef;
      border-color: #f2c6cd;
    }}

    .stats {{
      display: grid;
      grid-template-columns: repeat(3, 1fr);
      gap: 0.45rem;
    }}

    .stats div {{
      background: #f8fbf9;
      border: 1px solid #d8e7e0;
      border-radius: 10px;
      padding: 0.35rem 0.45rem;
      display: grid;
      gap: 0.1rem;
    }}

    .stats span {{
      font-size: 0.7rem;
      color: #5b6c77;
      text-transform: uppercase;
      letter-spacing: 0.03em;
    }}

    .stats strong {{
      font-size: 0.9rem;
      color: #173527;
    }}

    .delta {{
      margin: 0;
      font-size: 0.84rem;
      font-weight: 600;
    }}

    .delta.better {{ color: var(--good); }}
    .delta.worse {{ color: var(--bad); }}
    .delta.neutral {{ color: var(--neutral); }}

    .panel {{
      background: var(--card);
      border: 1px solid var(--line);
      border-radius: 16px;
      box-shadow: var(--shadow);
      padding: 0.8rem;
      display: grid;
      gap: 0.7rem;
    }}

    .panel-head {{
      display: flex;
      align-items: center;
      justify-content: space-between;
      flex-wrap: wrap;
      gap: 0.5rem;
    }}

    .panel h2 {{
      margin: 0;
      font-size: 1rem;
      color: #1f2f28;
    }}

    .phase-controls {{
      display: flex;
      flex-wrap: wrap;
      gap: 0.4rem;
    }}

    .phase-btn {{
      border: 1px solid #b8c8c0;
      background: #f5faf7;
      color: #244236;
      border-radius: 999px;
      font-size: 0.76rem;
      padding: 0.26rem 0.58rem;
      cursor: pointer;
      transition: transform 120ms ease, background 120ms ease;
    }}

    .phase-btn:hover {{ transform: translateY(-1px); }}
    .phase-btn.active {{
      background: #173f34;
      border-color: #173f34;
      color: #eaf7f1;
    }}

    .table-wrap {{
      overflow: auto;
      border: 1px solid #d3e1da;
      border-radius: 12px;
      background: #ffffff;
    }}

    table {{
      width: 100%;
      border-collapse: collapse;
      min-width: 980px;
    }}

    thead th {{
      position: sticky;
      top: 0;
      z-index: 2;
      background: #f1f8f4;
      color: #173527;
      border-bottom: 1px solid #d5e4dc;
      text-align: left;
      padding: 0.6rem;
      font-size: 0.78rem;
      font-weight: 700;
    }}

    thead small {{
      display: inline-block;
      margin-top: 0.2rem;
      color: #5d6f77;
      font-weight: 500;
      max-width: 260px;
      word-break: break-word;
      line-height: 1.25;
    }}

    tbody td {{
      border-bottom: 1px solid #edf3ef;
      padding: 0.55rem 0.6rem;
      vertical-align: top;
    }}

    .case-meta {{
      width: 280px;
      min-width: 260px;
    }}

    .case-id {{
      font-size: 0.88rem;
      font-weight: 700;
      color: #1d3c31;
    }}

    .case-alias {{
      margin-top: 0.12rem;
      font-size: 0.68rem;
      color: #67808c;
      line-height: 1.3;
    }}

    .case-script {{
      margin-top: 0.08rem;
      font-size: 0.7rem;
      color: #3f5e53;
      line-height: 1.3;
      word-break: break-word;
    }}

    .case-desc {{
      margin-top: 0.2rem;
      font-size: 0.75rem;
      color: #51626c;
      line-height: 1.35;
    }}

    .phase-tag {{
      display: inline-block;
      margin-top: 0.35rem;
      border: 1px solid #c7d8cf;
      border-radius: 999px;
      background: #f4faf6;
      color: #284639;
      font-size: 0.68rem;
      padding: 0.08rem 0.42rem;
      text-transform: uppercase;
      letter-spacing: 0.04em;
    }}

    td.metric.winner {{
      background: #f2faf5;
    }}

    .metric .median {{
      font-size: 0.86rem;
      font-weight: 700;
      color: #183429;
    }}

    .metric .status-pill {{
      display: inline-block;
      font-size: 0.66rem;
      font-weight: 700;
      border-radius: 999px;
      padding: 0.08rem 0.44rem;
      border: 1px solid transparent;
      letter-spacing: 0.02em;
      margin-bottom: 0.22rem;
      text-transform: uppercase;
    }}

    .metric .status-pill.status-rank-1 {{
      color: #1e5f3b;
      background: #e6f6ee;
      border-color: #bfe4d0;
    }}

    .metric .status-pill.status-rank-2 {{
      color: #8f2930;
      background: #fcecef;
      border-color: #f2c6cd;
    }}

    .metric .status-pill.status-rank-n {{
      color: #8f2930;
      background: #fcecef;
      border-color: #f2c6cd;
    }}

    .metric .status-pill.status-na {{
      color: #475467;
      background: #f3f4f6;
      border-color: #d6dae0;
    }}

    .metric .bar {{
      margin-top: 0.3rem;
      height: 0.3rem;
      border-radius: 999px;
      background: #eef4f1;
      overflow: hidden;
    }}

    .metric .bar span {{
      display: block;
      height: 100%;
      background: linear-gradient(90deg, var(--primary), #10b981);
    }}

    .metric .delta {{
      margin-top: 0.24rem;
      font-size: 0.72rem;
      color: #455864;
      font-weight: 600;
    }}

    .metric .ops {{
      margin-top: 0.08rem;
      font-size: 0.68rem;
      color: #61717a;
    }}

    @media (max-width: 760px) {{
      .container {{ width: calc(100vw - 1rem); margin: 0.6rem auto 1rem; }}
      .hero {{ border-radius: 14px; }}
      .panel {{ border-radius: 14px; }}
      .stats {{ grid-template-columns: 1fr 1fr 1fr; }}
      .phase-controls {{ width: 100%; }}
      .phase-btn {{ flex: 1 1 auto; text-align: center; }}
    }}
  </style>
</head>
<body>
  <main class=\"container\">
    <section class=\"hero\">
      <h1>{title}</h1>
      <div class=\"meta\">
        <span class=\"chip\">Generated: {generated_at}</span>
        <span class=\"chip\">Warmup: {config['warmup']}</span>
        <span class=\"chip\">Iterations: {config['iterations']}</span>
        <span class=\"chip\">Timeout: {config['timeout_seconds']}s</span>
        <span class=\"chip\">Reference: {html.escape(config['reference_app'])}</span>
      </div>
    </section>

    <section class=\"card-grid\">
      {''.join(summary_cards)}
    </section>

    <section class=\"panel\">
      <div class=\"panel-head\">
        <h2>Per-Case Comparison</h2>
        <div class=\"phase-controls\">{''.join(phase_buttons)}</div>
      </div>
      <div class=\"table-wrap\">
        <table id=\"cases-table\">
          <thead>
            <tr>
              <th>Case</th>
              {app_headers}
            </tr>
          </thead>
          <tbody>
            {''.join(case_rows_html)}
          </tbody>
        </table>
      </div>
    </section>
  </main>

  <script>
    (() => {{
      const buttons = Array.from(document.querySelectorAll('.phase-btn'));
      const rows = Array.from(document.querySelectorAll('#cases-table tbody tr'));

      const applyFilter = (phase) => {{
        rows.forEach((row) => {{
          const rowPhase = row.getAttribute('data-phase');
          const show = phase === 'all' || rowPhase === phase;
          row.style.display = show ? '' : 'none';
        }});
      }};

      buttons.forEach((btn) => {{
        btn.addEventListener('click', () => {{
          buttons.forEach((other) => other.classList.remove('active'));
          btn.classList.add('active');
          applyFilter(btn.getAttribute('data-phase'));
        }});
      }});
    }})();
  </script>
</body>
</html>
"""


def main() -> int:
    repo_root = Path(__file__).resolve().parents[1]
    runner_path = repo_root / "util" / "run.py"
    args, app_entries = parse_args()

    if not runner_path.exists():
        print(f"Error: benchmark runner not found at {runner_path}", file=sys.stderr)
        return 1

    app_reports = []
    try:
        for app_entry in app_entries:
            app_reports.append(
                run_single_benchmark(
                    repo_root,
                    runner_path,
                    int(app_entry["index"]),
                    str(app_entry["label"]),
                    Path(app_entry["path"]),
                    args,
                )
            )

        report = build_comparison(app_reports, args)

        json_out = Path(args.json_out).expanduser().resolve()
        html_out = Path(args.html_out).expanduser().resolve()
        json_out.parent.mkdir(parents=True, exist_ok=True)
        html_out.parent.mkdir(parents=True, exist_ok=True)

        json_out.write_text(json.dumps(report, indent=2), encoding="utf-8")
        html_out.write_text(render_html(report), encoding="utf-8")

        print(f"Wrote JSON comparison: {json_out}")
        print(f"Wrote HTML report: {html_out}")
    except CompareError as exc:
        print(f"Error: {exc}", file=sys.stderr)
        return 1
    except KeyboardInterrupt:
        print("Interrupted", file=sys.stderr)
        return 130

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
