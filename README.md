# cpp-limit-orderbook
High-performance Limit Order Book implementation in modern C++17 with price-time priority matching engine.

## Features

- **Matching Engine:** Price-time priority (FIFO), partial fills, resting order support
- **Order Class:** ID, type (BUY/SELL), price in cents, quantity, timestamp, immutable except `fill()`
- **OrderBook Container:** `std::map`-based price levels, FIFO queue per level via `std::vector<unique_ptr<Order>>`
- **Trade Struct:** Captures buy/sell IDs, executed price and quantity per fill
- **Memory Safety:** `unique_ptr` ownership throughout, no raw pointers
- **Modern C++17:** `std::optional`, `std::chrono`, `enum class`, structured bindings

## Tech Stack

- C++17
- GNU Make
- `std::map`: O(log n) price level access, supports Top-N iteration
- `std::vector`: FIFO queue per price level
- `std::unique_ptr`: deterministic memory management

## Build & Run

```bash
git clone https://github.com/Diego-2510/cpp-limit-orderbook.git
cd cpp-limit-orderbook
make
./orderbook_test
```

## Performance Benchmark (10k orders):

make benchmark
./benchmark

**Results (10,000 orders on ThinkPad E16 G3):**
- Mean latency: 0.87 μs
- Median (p50): 0.65 μs
- p99 latency: 2.34 μs
- Max latency: 18.21 μs

*Benchmark uses randomized BUY/SELL orders (prices: 95.00-105.00, quantities: 10-100). All orders processed via `match_order()` which handles matching + placement.*

## Project Structure

cpp-limit-orderbook/
├── include/
│ ├── order.h # Order class declaration + fill()
│ ├── orderbook.h # OrderBook with match_order()
│ └── trade.h # Trade value type
├── src/
│ ├── order.cpp
│ ├── orderbook.cpp # Matching engine implementation
│ ├── main.cpp # Functional tests
│ └── benchmark.cpp # 10k order benchmark
├── Makefile
└── README.md


## Design Rationale

**Why `std::map` over `priority_queue`?**  
`priority_queue` only exposes the top element → no iteration possible. `std::map` allows Top-N display and range iteration at O(log n) insertion. Acceptable for 10k orders.

**Why `std::greater` for bids?**  
Inverts map ordering so `.begin()` returns the highest bid in O(1) without extra logic.

**Why prices in `uint64_t` cents?**  
Avoids floating-point precision errors entirely (e.g. $100.50 → `10050`). Safe for arithmetic without `double` rounding.

**Why `unique_ptr` + `fill()` instead of immutable Orders?**  
Full immutability breaks partial fills. `fill()` is the minimal mutation needed → all other fields stay const-correct via getters only.

**Why FIFO per price level?**  
Standard exchange price-time priority: among orders at the same price, the earliest placed order is matched first. `vector::front()` + `erase(begin())` implements this directly.

## Matching Engine

`match_order(incoming)` executes against the opposing book side:
- **BUY:** matches against asks (lowest ask first), condition `incoming.price >= resting.price`
- **SELL:** matches against bids (highest bid first), condition `incoming.price <= resting.price`
- **Partial fills:** resting order stays in book with reduced quantity
- **Unmatched remainder:** placed into book via `add_order()`
- Returns `std::vector<Trade>` with all executed fills

## Roadmap

- [x] Week 1 - Order class with smart pointers
- [x] Week 2-3 - OrderBook container, best bid/ask, FIFO queues
- [x] Week 4-5 - Matching engine with partial fills
- [x] Week 6 - Benchmark + Final polish

## License

MIT License

## Author

Diego Ringleb
