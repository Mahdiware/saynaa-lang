# Saynaa Benchmark Suite

This directory contains phase-oriented benchmarks for Saynaa.

## Case Discovery

`util/run.py` auto-discovers benchmark cases from top-level files in `tests/benchmark/phases/*.sa`.

Metadata is read from header comments at the top of each benchmark file:

```ruby
# case_id="runtime.function_call"
# phase="runtime"
# description="Tight function call loop"
# mode="run-source"
# ops=250000
```

Header fields:
- required: `case_id`, `phase`, `description`, `mode`
- optional: `ops` (used for throughput)

If a file is missing required header fields, the runner infers safe defaults instead of crashing.

## Phases Covered

- compile: source to bytecode compilation
- runtime: loop math, function calls, method dispatch, attribute access, collections, string ops, module calls
- bytecode: precompiled bytecode execution

## Run Benchmarks

From repository root:

```bash
python3 util/run.py --app ./saynaa
```

With fewer runs for quick checks:

```bash
python3 util/run.py --app ./saynaa --warmup 1 --iterations 3
```

Run a specific phase:

```bash
python3 util/run.py --app ./saynaa --phase runtime
```

Run a single case:

```bash
python3 util/run.py --app ./saynaa --case runtime_function_call.sa
```

`--case` accepts either:
- case id (`runtime.method_dispatch`)
- benchmark file name (`runtime_method_dispatch.sa`)

When comparing binaries (`--app1`, `--app2`, ...), all apps run the same case files.

### Case Files Used By Both App1 and App2

- `bytecode_loop.sa`
- `compile_frontend.sa`
- `runtime_loop.sa`
- `runtime_function_call.sa`
- `runtime_method_dispatch.sa`
- `runtime_attribute_access.sa`
- `runtime_collections.sa`
- `runtime_string_ops.sa`
- `module_import.sa`

## Baseline Comparison

Save current report (default path is `tests/benchmark/results/latest.json`), then compare future runs:

```bash
python3 util/run.py --app ./saynaa --baseline tests/benchmark/results/latest.json
```

The runner prints per-case median, p95, standard deviation, throughput (when operation counts are known), and delta versus baseline.

## Regression Gate

Fail the run when selected benchmarks regress beyond a threshold:

```bash
python3 util/run.py \
	--app ./saynaa \
	--baseline tests/benchmark/results/latest.json \
	--fail-on-regression-pct 15
```

Gate only specific cases:

```bash
python3 util/run.py \
	--app ./saynaa \
	--baseline tests/benchmark/results/latest.json \
	--fail-on-regression-pct 15 \
	--gate-case runtime.loop \
	--gate-case runtime.method_dispatch
```

Require baseline coverage for gated cases:

```bash
python3 util/run.py \
	--app ./saynaa \
	--baseline tests/benchmark/results/latest.json \
	--fail-on-regression-pct 15 \
	--fail-on-missing-baseline
```

Exit status:
- `0`: success
- `1`: invalid inputs or runtime error
- `2`: regression gate failed

## Multi-App Comparison (HTML)

Compare two binaries and generate a user-friendly HTML report:

```bash
python3 util/compare.py \
	--app1 ./saynaa \
	--app2 /usr/local/bin/saynaa
```

Compare more binaries with incremental flags (`--app3`, `--app4`, ...):

```bash
python3 util/compare.py \
	--app1 ./saynaa \
	--app2 /usr/local/bin/saynaa \
	--app3 ./saynaa-experimental
```

Optional display names for report cards and table headers (`--name1`, `--name2`, ...):

```bash
python3 util/compare.py \
	--app1 ./saynaa --name1 local \
	--app2 /usr/local/bin/saynaa --name2 system
```

Custom output paths:

```bash
python3 util/compare.py \
	--app1 ./saynaa \
	--app2 /usr/local/bin/saynaa \
	--html-out tests/benchmark/results/my-compare.html \
	--json-out tests/benchmark/results/my-compare.json
```

The HTML report includes:
- per-app ranking cards
- wins per benchmark case
- geomean metrics vs reference app (`app1`)
- per-case latency, delta vs best, and throughput
- phase filters for quick browsing
