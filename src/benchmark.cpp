#include <iostream>
#include <chrono>
#include <memory>
#include <random>
#include <vector>
#include "order.h"
#include "orderbook.h"

using namespace std::chrono;

int main() {
    std::cout << "=== OrderBook Benchmark (10k Orders) ===\n\n";

    OrderBook book;
    std::mt19937 rng(42);
    std::uniform_int_distribution<uint64_t> price_dist(9500, 10500);
    std::uniform_int_distribution<uint64_t> qty_dist(10, 100);
    std::uniform_int_distribution<int> type_dist(0, 1);

    const int N = 10000;
    std::vector<duration<double, std::micro>> latencies;
    latencies.reserve(N);

    for (int i = 0; i < N; ++i) {
        OrderType type = type_dist(rng) == 0 ? OrderType::BUY : OrderType::SELL;
        uint64_t price = price_dist(rng);
        uint64_t qty = qty_dist(rng);

        auto order = std::make_unique<Order>(i, type, price, qty);

        auto start = high_resolution_clock::now();
        book.match_order(std::move(order));
        auto end = high_resolution_clock::now();

        latencies.push_back(duration<double, std::micro>(end - start));
    }

    std::sort(latencies.begin(), latencies.end());

    double sum = 0.0;
    for (const auto& lat : latencies) sum += lat.count();
    double mean = sum / N;
    double p50 = latencies[N / 2].count();
    double p99 = latencies[(99 * N) / 100].count();
    double max_lat = latencies.back().count();

    std::cout << "Orders processed: " << N << "\n";
    std::cout << "Mean latency:     " << mean << " μs\n";
    std::cout << "Median (p50):     " << p50 << " μs\n";
    std::cout << "p99 latency:      " << p99 << " μs\n";
    std::cout << "Max latency:      " << max_lat << " μs\n";
    std::cout << "\nFinal state:\n";
    std::cout << "  Bid levels: " << book.get_bid_levels() << "\n";
    std::cout << "  Ask levels: " << book.get_ask_levels() << "\n";

    return 0;
}
