#include "orderbook.h"

void OrderBook::add_order(std::unique_ptr<Order> order) {
    uint64_t price = order->getPrice();
    OrderType type = order->getType();
    
    if (type == OrderType::BUY) {
        bids_[price].push_back(std::move(order));
    } else {
        asks_[price].push_back(std::move(order));
    }
}

std::optional<uint64_t> OrderBook::get_best_bid() const {
    if (bids_.empty()) {
        return std::nullopt;
    }
    return bids_.begin()->first;  // highest price (std::greater)
}

std::optional<uint64_t> OrderBook::get_best_ask() const {
    if (asks_.empty()) {
        return std::nullopt;
    }
    return asks_.begin()->first;  // lowest price (std::less)
}

size_t OrderBook::get_bid_levels() const {
    return bids_.size();
}

size_t OrderBook::get_ask_levels() const {
    return asks_.size();
}
