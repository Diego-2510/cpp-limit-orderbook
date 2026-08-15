#include <iostream>
#include <memory>

#include "order.h"
#include "orderbook.h"

int main() {
    OrderBook book;

    book.add_order(std::make_unique<Order>(1, OrderType::SELL, 10100, 60));

    book.add_order(std::make_unique<Order>(2, OrderType::SELL, 10200, 40));

    const auto trades = book.match_order(std::make_unique<Order>(3, OrderType::BUY, 10200, 80));

    for (const auto& trade : trades) {
        std::cout << "trade buy=" << trade.buy_order_id << " sell=" << trade.sell_order_id
                  << " price=" << trade.price << " qty=" << trade.quantity << '\n';
    }

    if (const auto best_ask = book.get_best_ask()) {
        std::cout << "best ask after match: " << *best_ask << '\n';
    }

    return 0;
}