#include <iostream>
#include <memory>
#include "order.h"
#include "orderbook.h"

int main() {
    std::cout << "--- Basic OrderBook Test ---\n";
    OrderBook book;

    book.add_order(std::make_unique<Order>(1, OrderType::BUY,  10050, 100));
    book.add_order(std::make_unique<Order>(2, OrderType::BUY,  10100,  50));
    book.add_order(std::make_unique<Order>(3, OrderType::SELL, 10200,  75));

    if (auto b = book.get_best_bid()) std::cout << "Best bid: $" << *b / 100.0 << "\n";
    if (auto a = book.get_best_ask()) std::cout << "Best ask: $" << *a / 100.0 << "\n";

    // Edge Case: Empty Book
    std::cout << "\n--- Edge Case: Empty Book ---\n";
    OrderBook empty_book;
    if (!empty_book.get_best_bid() && !empty_book.get_best_ask())
        std::cout << "✓ Empty book returns nullopt correctly\n";

    // Matching Test: Full Fill
    std::cout << "\n--- Matching Test: Full Fill ---\n";
    OrderBook m1;
    m1.add_order(std::make_unique<Order>(10, OrderType::SELL, 10000, 100));
    auto trades = m1.match_order(std::make_unique<Order>(11, OrderType::BUY, 10000, 100));
    std::cout << "Trades executed: " << trades.size() << " (expected: 1)\n";
    if (!trades.empty())
        std::cout << "  qty=" << trades[0].quantity << " price=$" << trades[0].price / 100.0 << "\n";
    std::cout << "Ask levels after: " << m1.get_ask_levels() << " (expected: 0)\n";

    // Matching Test: Partial Fill
    std::cout << "\n--- Matching Test: Partial Fill ---\n";
    OrderBook m2;
    m2.add_order(std::make_unique<Order>(20, OrderType::SELL, 10000, 60));
    auto trades2 = m2.match_order(std::make_unique<Order>(21, OrderType::BUY, 10000, 100));
    std::cout << "Trades executed: " << trades2.size() << " (expected: 1)\n";
    if (!trades2.empty())
        std::cout << "  traded qty=" << trades2[0].quantity << " (expected: 60)\n";
    std::cout << "Bid levels after: " << m2.get_bid_levels() << " (expected: 1, rest=40)\n";

    // Matching Test: No Match (price too low)
    std::cout << "\n--- Matching Test: No Match ---\n";
    OrderBook m3;
    m3.add_order(std::make_unique<Order>(30, OrderType::SELL, 10500, 100));
    auto trades3 = m3.match_order(std::make_unique<Order>(31, OrderType::BUY, 10000, 100));
    std::cout << "Trades executed: " << trades3.size() << " (expected: 0)\n";
    std::cout << "Bid levels: " << m3.get_bid_levels() << " (expected: 1)\n";
    std::cout << "Ask levels: " << m3.get_ask_levels() << " (expected: 1)\n";

    return 0;
}
