# Benchmark Baselines

Optional CI baseline files live here.

- `linux-ci.json`: baseline used by `.github/workflows/build.yml` in the Linux benchmark job.

Generate or refresh it from a stable Linux environment:

```bash
python3 util/run.py \
  --app ./saynaa \
  --warmup 3 \
  --iterations 10 \
  --json-out tests/benchmark/baselines/linux-ci.json
```

Keep baseline updates intentional and infrequent to make regressions visible in pull requests.
