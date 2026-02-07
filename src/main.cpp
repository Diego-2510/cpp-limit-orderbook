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
    
    return 0;
}
