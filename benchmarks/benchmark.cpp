#include <algorithm>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <memory>
#include <random>
#include <vector>

#include "order.h"
#include "orderbook.h"

namespace {

struct OrderInput {
    uint32_t id;
    OrderType type;
    uint64_t price;
    uint32_t quantity;
};

std::vector<OrderInput> make_workload(std::size_t count, uint32_t first_id, uint64_t seed) {
    std::mt19937_64 rng(seed);

    std::uniform_int_distribution<uint64_t> price_dist(9500, 10500);

    std::uniform_int_distribution<uint32_t> quantity_dist(10, 100);

    std::uniform_int_distribution<int> side_dist(0, 1);

    std::vector<OrderInput> workload;
    workload.reserve(count);

    for (std::size_t i = 0; i < count; ++i) {
        workload.push_back(OrderInput{first_id + static_cast<uint32_t>(i),
                                      side_dist(rng) == 0 ? OrderType::BUY : OrderType::SELL,
                                      price_dist(rng), quantity_dist(rng)});
    }

    return workload;
}

void run_warmup(const std::vector<OrderInput>& workload) {
    OrderBook book;

    for (const auto& input : workload) {
        book.match_order(
            std::make_unique<Order>(input.id, input.type, input.price, input.quantity));
    }
}

double percentile(const std::vector<double>& sorted_ns, double p) {
    if (sorted_ns.empty()) {
        return 0.0;
    }

    const auto index = static_cast<std::size_t>(p * static_cast<double>(sorted_ns.size() - 1));

    return sorted_ns[index];
}

}  // namespace

int main() {
    constexpr std::size_t kWarmupOrders = 2'000;
    constexpr std::size_t kMeasuredOrders = 10'000;
    constexpr uint64_t kSeed = 42;

    const auto warmup = make_workload(kWarmupOrders, 1, kSeed);

    const auto measured = make_workload(kMeasuredOrders, 100'000, kSeed + 1);

    run_warmup(warmup);

    OrderBook book;

    std::vector<double> latencies_ns;
    latencies_ns.reserve(measured.size());

    for (const auto& input : measured) {
        auto order = std::make_unique<Order>(input.id, input.type, input.price, input.quantity);

        const auto start = std::chrono::steady_clock::now();

        book.match_order(std::move(order));

        const auto end = std::chrono::steady_clock::now();

        latencies_ns.push_back(std::chrono::duration<double, std::nano>(end - start).count());
    }

    std::sort(latencies_ns.begin(), latencies_ns.end());

    double sum_ns = 0.0;

    for (const double latency_ns : latencies_ns) {
        sum_ns += latency_ns;
    }

    const double mean_ns = sum_ns / static_cast<double>(latencies_ns.size());

    std::cout << std::fixed << std::setprecision(2);

    std::cout << "=== OrderBook benchmark ===\n";

    std::cout << "warm-up orders:  " << kWarmupOrders << '\n';

    std::cout << "measured orders: " << kMeasuredOrders << '\n';

    std::cout << "seed:            " << kSeed << '\n';

#if defined(__clang__)
    std::cout << "compiler:        Clang " << __clang_version__ << '\n';
#elif defined(__GNUC__)
    std::cout << "compiler:        GCC " << __VERSION__ << '\n';
#else
    std::cout << "compiler:        unknown\n";
#endif

    std::cout << "mean:            " << mean_ns << " ns\n";

    std::cout << "p50:             " << percentile(latencies_ns, 0.50) << " ns\n";

    std::cout << "p95:             " << percentile(latencies_ns, 0.95) << " ns\n";

    std::cout << "p99:             " << percentile(latencies_ns, 0.99) << " ns\n";

    std::cout << "p99.9:           " << percentile(latencies_ns, 0.999) << " ns\n";

    std::cout << "max:             " << latencies_ns.back() << " ns\n";

    std::cout << "final bid levels: " << book.get_bid_levels() << '\n';

    std::cout << "final ask levels: " << book.get_ask_levels() << '\n';

    return 0;
}