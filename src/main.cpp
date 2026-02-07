#include <iostream>
#include <memory>
#include "order.h"
#include "orderbook.h"

int main() {
    // Basic functionality test
    std::cout << "--- Basic OrderBook Test ---\n";
    OrderBook book;
    
    book.add_order(std::make_unique<Order>(1, OrderType::BUY, 10050, 100));
    book.add_order(std::make_unique<Order>(2, OrderType::BUY, 10100, 50));
    book.add_order(std::make_unique<Order>(3, OrderType::SELL, 10200, 75));
    
    auto best_bid = book.get_best_bid();
    auto best_ask = book.get_best_ask();
    
    std::cout << "Bid levels: " << book.get_bid_levels() << "\n";
    std::cout << "Ask levels: " << book.get_ask_levels() << "\n";
    
    if (best_bid) {
        std::cout << "Best bid: $" << *best_bid / 100.0 << "\n";
    }
    
    if (best_ask) {
        std::cout << "Best ask: $" << *best_ask / 100.0 << "\n";
    }
    
    // FIFO Test
    std::cout << "\n--- FIFO Test ---\n";
    OrderBook fifo_book;
    
    fifo_book.add_order(std::make_unique<Order>(10, OrderType::BUY, 10100, 50));
    fifo_book.add_order(std::make_unique<Order>(11, OrderType::BUY, 10100, 30));
    fifo_book.add_order(std::make_unique<Order>(12, OrderType::BUY, 10100, 20));
    
    std::cout << "Added 3 BUY orders at $101.00 with IDs 10, 11, 12\n";
    std::cout << "Expected: FIFO order (10 -> 11 -> 12)\n";
    std::cout << "Note: Currently no getter to verify vector order\n";
    
    // Edge Case: Empty OrderBook
    std::cout << "\n--- Edge Case: Empty Book ---\n";
    OrderBook empty_book;
    
    auto empty_bid = empty_book.get_best_bid();
    auto empty_ask = empty_book.get_best_ask();
    
    if (!empty_bid && !empty_ask) {
        std::cout << "✓ Empty book returns nullopt correctly\n";
    } else {
        std::cout << "✗ Error: Expected nullopt for empty book\n";
    }
    
    return 0;
}
