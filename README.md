# C++ Limit Order Book

A compact C++17 limit order book that demonstrates deterministic
price-time-priority matching, partial fills, explicit ownership, and
reproducible correctness checks.

This repository is intentionally small. It is the baseline implementation
that will later evolve into a broader exchange core; it does **not** claim
production-exchange or low-latency readiness.

## What It Implements

- Price priority across bid and ask levels
- FIFO arrival order within one price level
- Full and partial fills
- Matching at the resting order's price
- Resting unmatched quantities
- Best bid and best ask queries
- Explicit `Trade` events returned by each match operation
- Integer prices to avoid floating-point price representation
- `std::unique_ptr` ownership for orders
- Defensive validation against null orders, zero-quantity orders, and
  overfills

## Architecture

```text
incoming Order
     |
     v
+------------+       trades
| OrderBook  | -----------------> vector<Trade>
+------------+
   |      |
   |      +---- asks: std::map<price, FIFO vector>
   +----------- bids: std::map<price, FIFO vector, descending>
```

The highest bid and lowest ask are available from the first map element.

Within a price level, insertion order defines FIFO priority.

## Requirements

- CMake 3.20+
- A C++17 compiler
- GCC or Clang recommended
- Git during the first test configuration so CMake can fetch GoogleTest

## Quick Start

```bash
git clone https://github.com/Diego-2510/cpp-limit-orderbook.git
cd cpp-limit-orderbook

cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --parallel
ctest --test-dir build --output-on-failure

./build/orderbook_demo
```

The first test configuration fetches GoogleTest `v1.17.0` through CMake
`FetchContent`.

## Convenience Make Targets

The `Makefile` is only a thin wrapper around CMake. CMake remains the source
of truth for the build.

```bash
make
make test
make benchmark
make clean
```

## Correctness Tests

The GoogleTest suite covers:

- empty-book behavior
- best bid and best ask ordering
- full fills
- partial fills on incoming orders
- partial fills on resting orders
- non-crossing orders
- FIFO within a price level
- sweeping multiple price levels
- sell-side matching against the highest bid first
- execution at the resting order's price
- trade order IDs
- invalid null orders
- zero-quantity rejection
- overfill protection

Run locally:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --parallel
ctest --test-dir build --output-on-failure
```

## Sanitizers

AddressSanitizer and UndefinedBehaviorSanitizer can be enabled explicitly:

```bash
cmake \
  -S . \
  -B build-sanitized \
  -DCMAKE_BUILD_TYPE=Debug \
  -DORDERBOOK_ENABLE_SANITIZERS=ON

cmake \
  --build build-sanitized \
  --parallel

ctest \
  --test-dir build-sanitized \
  --output-on-failure
```

GitHub Actions executes this configuration automatically.

## Static Analysis

The repository contains `.clang-format` and `.clang-tidy` configurations.

CI checks formatting and compiles the project with clang-tidy enabled.

## Benchmark

The benchmark is deliberately presented as a reproducible microbenchmark,
not as evidence of exchange-grade latency.

Run:

```bash
./scripts/run_benchmark.sh
```

The benchmark performs a warm-up and reports:

- mean
- p50
- p95
- p99
- p99.9
- maximum latency

for a deterministic randomized workload.

The benchmark script also prints machine and toolchain metadata.

See [`benchmarks/README.md`](benchmarks/README.md) for methodology and
limitations.

No fixed latency claim is published in this README because benchmark results
are only meaningful together with the exact hardware, compiler, flags,
workload, and raw measurements used to produce them.

## Build Options

| Option | Default | Purpose |
|---|---:|---|
| `BUILD_TESTING` | `ON` | Build GoogleTest-based tests |
| `ORDERBOOK_BUILD_DEMO` | `ON` | Build `orderbook_demo` |
| `ORDERBOOK_BUILD_BENCHMARK` | `ON` | Build `orderbook_benchmark` |
| `ORDERBOOK_ENABLE_SANITIZERS` | `OFF` | Enable ASan and UBSan |
| `ORDERBOOK_ENABLE_CLANG_TIDY` | `OFF` | Run clang-tidy |
| `ORDERBOOK_WARNINGS_AS_ERRORS` | `ON` | Promote warnings to errors |

## Project Structure

```text
.
├── .github/
│   ├── dependabot.yml
│   └── workflows/
│       ├── ci.yml
│       └── codeql.yml
├── benchmarks/
│   ├── README.md
│   └── benchmark.cpp
├── examples/
│   └── demo.cpp
├── include/
│   ├── order.h
│   ├── orderbook.h
│   └── trade.h
├── scripts/
│   └── run_benchmark.sh
├── src/
│   ├── order.cpp
│   └── orderbook.cpp
├── tests/
│   ├── order_tests.cpp
│   └── orderbook_tests.cpp
├── .clang-format
├── .clang-tidy
├── .gitignore
├── CMakeLists.txt
├── Makefile
├── LICENSE
└── README.md
```

## Design Decisions

### Integer Prices

Prices are stored as `uint64_t`.

The caller defines the scale, for example cents.

This avoids binary floating-point representation errors inside the matching
engine.

### Price-Level Maps

Bids use a descending `std::map`, while asks use ascending order.

This keeps best-price access simple and deterministic while price-level
lookup and insertion remain O(log P), where `P` is the number of price
levels.

### FIFO at a Price Level

The current implementation stores orders in:

```cpp
std::vector<std::unique_ptr<Order>>
```

This preserves arrival order.

However, removing the first element with:

```cpp
erase(begin())
```

has O(n) complexity.

This is a documented limitation rather than a hidden performance issue.

## Known Limitations

- No cancel operation
- No replace operation
- No active-order ID index
- No duplicate-order-ID rejection
- No market orders
- No IOC orders
- No FOK orders
- No post-only orders
- No persistence
- No deterministic event replay
- No exchange protocol parser
- Single-threaded only
- FIFO front removal is O(n)
- Benchmark results are not production latency guarantees

These limitations are intentional boundaries of this repository.

A future exchange-core implementation can change the underlying data
structures and feature set behind stronger invariants and broader tests.

## CI and Security

GitHub Actions runs:

- GCC build
- Clang build
- GoogleTest
- AddressSanitizer
- UndefinedBehaviorSanitizer
- clang-format
- clang-tidy
- CodeQL C/C++ analysis

## License

MIT License.

See [`LICENSE`](LICENSE).