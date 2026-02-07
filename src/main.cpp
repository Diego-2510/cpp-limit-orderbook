#include <iostream>
#include <memory>
#include "order.h"
#include "orderbook.h"

int main() {
    OrderBook book;
    
    // Test case
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
    
    return 0;
}
