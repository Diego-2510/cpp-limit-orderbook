# cpp-limit-orderbook
High-performance Limit Order Book implementation in modern C++.

## Current Status
Week 2/6 - OrderBook Container Implementation
- Order class with immutable design ✓
- OrderBook with std::map-based storage ✓
- FIFO queues per price level ✓
- Best bid/ask getters ✓
- Matching engine (coming Week 4-5)

## Features
- **Order Class:** ID, type (BUY/SELL), price, quantity, timestamp
- **OrderBook Container:** Map-based storage with FIFO queue per price level
- **Price-Time Priority:** Bids sorted descending (std::greater), Asks ascending (std::less)
- **Memory Safety:** unique_ptr ownership, no raw pointers
- **Modern C++17:** std::chrono timestamps, std::optional, enum class

## Tech Stack
- C++17
- GNU Make
- std::map for O(log n) price level access
- std::vector for FIFO queues

## Build
```bash
git clone https://github.com/Diego-2510/cpp-limit-orderbook.git
cd cpp-limit-orderbook
make
```

## Design Rationale
**Why immutable Orders?**
Orders don't change after creation, making them safer for concurrent OrderBook access.

**Why enum class OrderType?**
Type-safe (no implicit int conversion), minimal memory footprint (1 byte storage).

**Why uint64_t for price?**
Store prices in cents (e.g., $100.50 = 10050) to avoid floating-point precision errors.

**Why std::map over priority_queue?**
Map allows iteration over top N price levels (needed for order book display). Priority_queue only provides top element access. O(log n) insertion is acceptable for 10k orders.

**Why std::greater for bids?**
Bids need highest price first. std::greater inverts map ordering so `.begin()` returns best bid in O(1).

## Roadmap
- Week 1: Order class ✓
- Week 2-3: OrderBook with std::map ✓
- Week 4-5: Matching engine
- Week 6: Benchmark (10k orders target)

## License
MIT License

## Author
Diego Ringleb