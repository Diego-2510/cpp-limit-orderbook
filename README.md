# cpp-limit-orderbook
High-performance Limit Order Book implementation in modern C++.

## Current Status
Week 1/6 - Order Class Implementation
- Order class with immutable design
- Smart pointer-ready architecture
- Type-safe OrderType enum
- OrderBook container (coming Week 2-3)
- Matching engine (coming Week 4-5)

## Features
- Order Class: ID, type (BUY/SELL), price, quantity, timestamp
- Memory Safety: Designed for unique_ptr ownership
- Modern C++17: std::chrono timestamps, enum class

## Tech Stack
- C++17
- GNU Make
- std::chrono for high-precision timestamps

## Build
```bash
git clone https://github.com/Diego-2510/cpp-limit-orderbook.git
cd cpp-limit-orderbook
make
```

## Design Rationale
Why immutable Orders?
Orders don't change after creation, making them safer for concurrent OrderBook access.

Why enum class OrderType?
Type-safe (no implicit int conversion), minimal memory footprint (1 byte storage).

Why uint64_t for price?
Store prices in cents (e.g., $100.50 = 10050) to avoid floating-point precision errors.

## Roadmap
- Week 1: Order class
- Week 2-3: OrderBook with std::map
- Week 4-5: Matching engine
- Week 6: Benchmark (10k orders target)

## License
MIT License

## Author
Diego Ringleb
