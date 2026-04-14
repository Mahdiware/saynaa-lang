#!/usr/bin/env python3

import argparse
import json
import platform
import re
import statistics
import subprocess
import sys
import tempfile
import time
from dataclasses import dataclass
from datetime import datetime, timezone
from pathlib import Path
from typing import Dict, List, Optional, Set, Tuple


@dataclass(frozen=True)
class BenchCase:
    case_id: str
    phase: str
    description: str
    script: Path
    mode: str  # run-source | compile-only | run-bytecode
    ops: Optional[int] = None


HEADER_KV_RE = re.compile(r"^\s*#\s*([a-zA-Z_][\w-]*)\s*=\s*(.+?)\s*$")
VALID_MODES = {"run-source", "compile-only", "run-bytecode"}


def percentile(values: List[float], p: float) -> float:
    if not values:
        return 0.0
    ordered = sorted(values)
    if len(ordered) == 1:
        return ordered[0]
    rank = (len(ordered) - 1) * (p / 100.0)
    lo = int(rank)
    hi = min(lo + 1, len(ordered) - 1)
    if lo == hi:
        return ordered[lo]
    weight = rank - lo
    return ordered[lo] + (ordered[hi] - ordered[lo]) * weight


def parse_header_value(raw_value: str) -> str:
    value = raw_value.strip()
    if len(value) >= 2 and value[0] == value[-1] and value[0] in ("\"", "'"):
        return value[1:-1]
    return value


def parse_case_header(script: Path) -> Dict[str, str]:
    header: Dict[str, str] = {}
    lines = script.read_text(encoding="utf-8").splitlines()

    for line in lines:
        stripped = line.strip()
        if not stripped:
            if header:
                break
            continue
        if not stripped.startswith("#"):
            break

        match = HEADER_KV_RE.match(line)
        if not match:
            continue

        key = match.group(1).strip().lower()
        value = parse_header_value(match.group(2))
        header[key] = value

    return header


def infer_case_defaults(script: Path) -> Dict[str, str]:
    stem = script.stem
    phase = "runtime"
    suffix = stem

    if "_" in stem:
        prefix, remainder = stem.split("_", 1)
        if prefix in {"compile", "runtime", "bytecode"} and remainder:
            phase = prefix
            suffix = remainder

    mode = "run-source"
    if phase == "compile":
        mode = "compile-only"
    elif phase == "bytecode":
        mode = "run-bytecode"

    description = " ".join(suffix.split("_")).strip()
    description = description[:1].upper() + description[1:] if description else f"{phase} case"

    return {
        "case_id": f"{phase}.{suffix}",
        "phase": phase,
        "description": description,
        "mode": mode,
    }


def parse_ops_value(raw_ops: str, script: Path, warnings: List[str]) -> Optional[int]:
    try:
        ops = int(raw_ops)
    except ValueError:
        warnings.append(
            f"{script.name}: invalid ops value '{raw_ops}', throughput will be disabled"
        )
        return None

    if ops <= 0:
        warnings.append(
            f"{script.name}: ops must be > 0, throughput will be disabled"
        )
        return None

    return ops


def build_case_from_script(script: Path, warnings: List[str]) -> BenchCase:
    header = parse_case_header(script)
    defaults = infer_case_defaults(script)

    missing_fields = []
    for field in ("case_id", "phase", "description", "mode"):
        if field not in header:
            missing_fields.append(field)
    if missing_fields:
        warnings.append(
            f"{script.name}: missing header field(s) "
            + ", ".join(missing_fields)
            + "; inferred defaults were used"
        )

    case_id = header.get("case_id", defaults["case_id"]).strip()
    phase = header.get("phase", defaults["phase"]).strip().lower()
    description = header.get("description", defaults["description"]).strip()
    mode = header.get("mode", defaults["mode"]).strip().lower()

    if not case_id:
        case_id = defaults["case_id"]
    if not phase:
        phase = defaults["phase"]
    if not description:
        description = defaults["description"]
    if mode not in VALID_MODES:
        warnings.append(
            f"{script.name}: unsupported mode '{mode}', falling back to '{defaults['mode']}'"
        )
        mode = defaults["mode"]

    ops = None
    if "ops" in header:
        ops = parse_ops_value(header["ops"], script, warnings)

    return BenchCase(
        case_id=case_id,
        phase=phase,
        description=description,
        script=script,
        mode=mode,
        ops=ops,
    )


def discover_cases(repo_root: Path) -> List[BenchCase]:
    phases_dir = repo_root / "tests" / "benchmark" / "phases"
    if not phases_dir.exists():
        raise FileNotFoundError(f"Benchmark phases directory not found: {phases_dir}")

    scripts = sorted(phases_dir.glob("*.sa"))
    if not scripts:
        raise FileNotFoundError(f"No benchmark scripts found in: {phases_dir}")

    warnings: List[str] = []
    cases: List[BenchCase] = []
    seen_case_ids: Dict[str, Path] = {}

    for script in scripts:
        case = build_case_from_script(script, warnings)
        existing = seen_case_ids.get(case.case_id)
        if existing is not None:
            raise ValueError(
                f"Duplicate case_id '{case.case_id}' in {existing.name} and {script.name}"
            )
        seen_case_ids[case.case_id] = script
        cases.append(case)

    for warning in warnings:
        print(f"Warning: {warning}", file=sys.stderr)

    return cases


def run_once(cmd: List[str], timeout: float) -> float:
    start = time.perf_counter()
    try:
        proc = subprocess.run(
            cmd,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
            timeout=timeout,
            check=False,
        )
    except subprocess.TimeoutExpired as exc:
        raise RuntimeError(f"Timed out after {timeout:.1f}s: {' '.join(cmd)}") from exc

    elapsed = time.perf_counter() - start
    if proc.returncode != 0:
        stderr = proc.stderr.strip()
        stdout = proc.stdout.strip()
        detail = stderr if stderr else stdout
        raise RuntimeError(
            f"Command failed ({proc.returncode}): {' '.join(cmd)}\n{detail}"
        )

    return elapsed


def build_command(
    case: BenchCase,
    app_path: Path,
    temp_dir: Path,
    bytecode_cache: Dict[str, Path],
    timeout: float,
) -> List[str]:
    if case.mode == "run-source":
        return [str(app_path), str(case.script)]

    if case.mode == "compile-only":
        out_path = temp_dir / f"{case.case_id.replace('.', '_')}.sbc"
        return [str(app_path), "-b", "-o", str(out_path), str(case.script)]

    if case.mode == "run-bytecode":
        if case.case_id not in bytecode_cache:
            out_path = temp_dir / f"{case.case_id.replace('.', '_')}.sbc"
            compile_cmd = [str(app_path), "-b", "-o", str(out_path), str(case.script)]
            run_once(compile_cmd, timeout)
            bytecode_cache[case.case_id] = out_path
        return [str(app_path), str(bytecode_cache[case.case_id])]

    raise ValueError(f"Unknown benchmark mode: {case.mode}")


def load_baseline(path: Optional[Path]) -> Dict[str, float]:
    if path is None:
        return {}
    if not path.exists():
        raise FileNotFoundError(f"Baseline file does not exist: {path}")
    data = json.loads(path.read_text(encoding="utf-8"))
    baseline = {}
    for item in data.get("results", []):
        case_id = item.get("id")
        median_ms = item.get("median_ms")
        if isinstance(case_id, str) and isinstance(median_ms, (int, float)):
            baseline[case_id] = float(median_ms)
    return baseline


def evaluate_regressions(
    results: List[dict],
    threshold_pct: float,
    gated_case_ids: Set[str],
) -> Tuple[List[dict], List[str]]:
    violations = []
    missing_baseline = []

    for row in results:
        case_id = row.get("id")
        if not isinstance(case_id, str):
            continue
        if gated_case_ids and case_id not in gated_case_ids:
            continue

        baseline_ms = row.get("baseline_median_ms")
        if baseline_ms is None:
            missing_baseline.append(case_id)
            continue

        delta_pct = row.get("delta_percent")
        if isinstance(delta_pct, (int, float)) and delta_pct > threshold_pct:
            violations.append(
                {
                    "id": case_id,
                    "delta_percent": float(delta_pct),
                    "median_ms": row.get("median_ms"),
                    "baseline_median_ms": baseline_ms,
                }
            )

    return violations, missing_baseline


def build_case_alias_map(cases: List[BenchCase]) -> Dict[str, Set[str]]:
    alias_map: Dict[str, Set[str]] = {}
    for case in cases:
        for alias in (case.case_id, case.script.name):
            alias_map.setdefault(alias, set()).add(case.case_id)
    return alias_map


def resolve_case_selectors(
    selectors: List[str],
    alias_map: Dict[str, Set[str]],
) -> Tuple[Set[str], List[str]]:
    resolved: Set[str] = set()
    unknown: List[str] = []
    for selector in selectors:
        case_ids = alias_map.get(selector)
        if not case_ids:
            unknown.append(selector)
            continue
        resolved.update(case_ids)
    return resolved, unknown


def main() -> int:
    repo_root = Path(__file__).resolve().parents[1]
    default_app = repo_root / ("saynaa.exe" if platform.system() == "Windows" else "saynaa")
    default_json = repo_root / "tests" / "benchmark" / "results" / "latest.json"

    parser = argparse.ArgumentParser(description="Saynaa benchmark runner")
    parser.add_argument("--app", default=str(default_app), help="Path to saynaa executable")
    parser.add_argument("--warmup", type=int, default=3, help="Warmup runs per case")
    parser.add_argument("--iterations", type=int, default=10, help="Measured runs per case")
    parser.add_argument("--timeout", type=float, default=30.0, help="Timeout per run in seconds")
    parser.add_argument("--phase", action="append", default=[], help="Run only selected phase(s)")
    parser.add_argument(
        "--case",
        action="append",
        default=[],
        help="Run only selected case id(s) or script name(s), e.g. runtime_function_call.sa",
    )
    parser.add_argument("--json-out", default=str(default_json), help="Write JSON report to path")
    parser.add_argument("--baseline", default=None, help="Compare against a previous JSON report")
    parser.add_argument(
        "--fail-on-regression-pct",
        type=float,
        default=None,
        help="Fail if any gated case regresses by more than this percent versus baseline",
    )
    parser.add_argument(
        "--gate-case",
        action="append",
        default=[],
        help="Case id(s) or script name(s) included in regression gate (repeatable)",
    )
    parser.add_argument(
        "--fail-on-missing-baseline",
        action="store_true",
        help="Fail when a gated case has no baseline entry",
    )
    args = parser.parse_args()

    app_path = Path(args.app).resolve()
    if not app_path.exists():
        print(f"Error: executable not found: {app_path}", file=sys.stderr)
        print("Build first: make MODE=RELEASE all", file=sys.stderr)
        return 1

    try:
        cases = discover_cases(repo_root)
    except (FileNotFoundError, ValueError, OSError, UnicodeError) as exc:
        print(f"Error: {exc}", file=sys.stderr)
        return 1

    if args.phase:
        selected = set(args.phase)
        cases = [case for case in cases if case.phase in selected]

    alias_map = build_case_alias_map(cases)

    if args.case:
        selected_case_ids, unknown_case_selectors = resolve_case_selectors(args.case, alias_map)
        if unknown_case_selectors:
            print(
                "Error: unknown --case value(s): " + ", ".join(sorted(unknown_case_selectors)),
                file=sys.stderr,
            )
            return 1
        cases = [case for case in cases if case.case_id in selected_case_ids]

    if not cases:
        print("No benchmark cases selected.", file=sys.stderr)
        return 1

    gated_case_ids: Set[str] = set()
    if args.gate_case:
        gate_alias_map = build_case_alias_map(cases)
        gated_case_ids, unknown_gate_cases = resolve_case_selectors(args.gate_case, gate_alias_map)
        if unknown_gate_cases:
            print(
                "Error: unknown --gate-case value(s): " + ", ".join(sorted(unknown_gate_cases)),
                file=sys.stderr,
            )
            return 1

    try:
        baseline = load_baseline(Path(args.baseline).resolve() if args.baseline else None)
    except (FileNotFoundError, json.JSONDecodeError) as exc:
        print(f"Error: {exc}", file=sys.stderr)
        return 1

    if args.fail_on_regression_pct is not None and args.baseline is None:
        print("Error: --fail-on-regression-pct requires --baseline", file=sys.stderr)
        return 1
    if args.fail_on_regression_pct is not None and args.fail_on_regression_pct < 0:
        print("Error: --fail-on-regression-pct must be >= 0", file=sys.stderr)
        return 1

    print("Saynaa Benchmark Runner")
    print(f"App: {app_path}")
    print(f"Warmup: {args.warmup} | Iterations: {args.iterations} | Cases: {len(cases)}")
    if baseline:
        print(f"Baseline: {Path(args.baseline).resolve()}")
    if args.fail_on_regression_pct is not None:
        gate_scope = ", ".join(args.gate_case) if args.gate_case else "all selected cases"
        print(
            f"Regression gate: >{args.fail_on_regression_pct:.2f}% on {gate_scope}"
        )

    report_results = []

    with tempfile.TemporaryDirectory(prefix="saynaa-bench-") as temp_dir_name:
        temp_dir = Path(temp_dir_name)
        bytecode_cache: Dict[str, Path] = {}

        for case in cases:
            # Warmup
            for _ in range(args.warmup):
                cmd = build_command(case, app_path, temp_dir, bytecode_cache, args.timeout)
                run_once(cmd, args.timeout)

            # Measure
            samples_sec = []
            for _ in range(args.iterations):
                cmd = build_command(case, app_path, temp_dir, bytecode_cache, args.timeout)
                samples_sec.append(run_once(cmd, args.timeout))

            samples_ms = [v * 1000.0 for v in samples_sec]
            median_ms = statistics.median(samples_ms)
            mean_ms = statistics.fmean(samples_ms)
            min_ms = min(samples_ms)
            max_ms = max(samples_ms)
            p95_ms = percentile(samples_ms, 95.0)
            stddev_ms = statistics.stdev(samples_ms) if len(samples_ms) > 1 else 0.0

            ops_per_sec = None
            if case.ops and median_ms > 0.0:
                ops_per_sec = case.ops / (median_ms / 1000.0)

            baseline_ms = baseline.get(case.case_id)
            delta_pct = None
            if baseline_ms is not None and baseline_ms > 0:
                delta_pct = ((median_ms - baseline_ms) / baseline_ms) * 100.0

            row = {
                "id": case.case_id,
                "case_file": case.script.name,
                "script": str(case.script.relative_to(repo_root)),
                "phase": case.phase,
                "description": case.description,
                "mode": case.mode,
                "samples_ms": samples_ms,
                "mean_ms": mean_ms,
                "median_ms": median_ms,
                "p95_ms": p95_ms,
                "stddev_ms": stddev_ms,
                "min_ms": min_ms,
                "max_ms": max_ms,
                "ops_per_sec": ops_per_sec,
                "baseline_median_ms": baseline_ms,
                "delta_percent": delta_pct,
            }
            report_results.append(row)

            ops_text = f"{ops_per_sec:,.0f} ops/s" if ops_per_sec is not None else "-"
            delta_text = f"{delta_pct:+.2f}%" if delta_pct is not None else "-"
            case_label = case.script.name
            print(
                f"[{case.phase:8}] {case_label:24} median={median_ms:8.3f}ms "
                f"p95={p95_ms:8.3f}ms std={stddev_ms:8.3f}ms "
                f"throughput={ops_text:>14} delta={delta_text}"
            )

    report = {
        "generated_at": datetime.now(timezone.utc).isoformat(),
        "system": {
            "platform": platform.platform(),
            "machine": platform.machine(),
            "python": platform.python_version(),
        },
        "config": {
            "app": str(app_path),
            "warmup": args.warmup,
            "iterations": args.iterations,
            "timeout_seconds": args.timeout,
            "selected_phases": args.phase,
            "selected_cases": args.case,
            "baseline": str(Path(args.baseline).resolve()) if args.baseline else None,
            "fail_on_regression_pct": args.fail_on_regression_pct,
            "gate_cases": args.gate_case,
            "gate_case_ids": sorted(gated_case_ids),
            "fail_on_missing_baseline": args.fail_on_missing_baseline,
        },
        "results": report_results,
    }

    violations = []
    missing_baseline = []
    if args.fail_on_regression_pct is not None:
        violations, missing_baseline = evaluate_regressions(
            report_results,
            args.fail_on_regression_pct,
            gated_case_ids,
        )
        report["regression_gate"] = {
            "threshold_percent": args.fail_on_regression_pct,
            "gated_case_ids": sorted(gated_case_ids),
            "gated_case_inputs": args.gate_case,
            "violations": violations,
            "missing_baseline": missing_baseline,
            "pass": len(violations) == 0
            and (not args.fail_on_missing_baseline or len(missing_baseline) == 0),
        }

    json_out = Path(args.json_out).resolve()
    json_out.parent.mkdir(parents=True, exist_ok=True)
    json_out.write_text(json.dumps(report, indent=2), encoding="utf-8")

    print(f"\nWrote benchmark report: {json_out}")

    if args.fail_on_regression_pct is not None:
        if missing_baseline:
            print(
                "Regression gate: missing baseline for "
                + ", ".join(sorted(missing_baseline))
            )
        if violations:
            print("Regression gate violations:")
            for issue in sorted(
                violations, key=lambda row: row["delta_percent"], reverse=True
            ):
                print(
                    "- "
                    + f"{issue['id']}: {issue['delta_percent']:+.2f}% "
                    + f"(median={issue['median_ms']:.3f}ms, baseline={issue['baseline_median_ms']:.3f}ms)"
                )

        if violations:
            return 2
        if missing_baseline and args.fail_on_missing_baseline:
            return 2

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
