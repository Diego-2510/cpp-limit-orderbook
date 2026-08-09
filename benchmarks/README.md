# Benchmark Methodology

The benchmark is a deterministic microbenchmark for the current matching path.

It is not a claim about exchange-grade, production, or HFT latency.

## Workload

- 2,000 warm-up orders
- 10,000 measured orders
- fixed random seeds
- BUY/SELL side selected uniformly
- integer prices in `[9500, 10500]`
- quantities in `[10, 100]`
- `Order` allocation occurs before the timed `match_order()` call
- the order book evolves throughout the measured run

The executable reports:

- mean
- p50
- p95
- p99
- p99.9
- maximum latency

Timing uses `std::chrono::steady_clock`.

## Reproduce

```bash
./scripts/run_benchmark.sh
```

For results intended for public documentation, run the benchmark multiple
times on an otherwise idle machine and record:

- CPU model
- operating system and kernel
- compiler and version
- CMake version
- build type and flags
- all raw runs, including slower runs

Do not compare results across different machines, compiler flags, or
workloads as if they were equivalent.

## Known Limitations

Timer overhead is included in every measurement.

CPU frequency scaling, scheduling activity, thermal state, and background
processes can affect tail latency.

The order book evolves during the benchmark, so individual measurements are
not independent and identically distributed.

The current FIFO container uses `std::vector`. Removing a completely filled
resting order with `erase(begin())` therefore has O(n) complexity within the
price level.

A future exchange-core implementation will replace this data structure before
making stronger latency claims.