#include "orderbook.h"
#include <algorithm>

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
    if (bids_.empty()) return std::nullopt;
    return bids_.begin()->first;
}

std::optional<uint64_t> OrderBook::get_best_ask() const {
    if (asks_.empty()) return std::nullopt;
    return asks_.begin()->first;
}

size_t OrderBook::get_bid_levels() const {
    return bids_.size();
}

size_t OrderBook::get_ask_levels() const {
    return asks_.size();
}

std::vector<Trade> OrderBook::match_order(std::unique_ptr<Order> incoming) {
    std::vector<Trade> trades;

    bool is_buy = (incoming->getType() == OrderType::BUY);

    while (incoming->getQuantity() > 0) {
        if (is_buy) {
            if (asks_.empty()) break;
            auto level_it = asks_.begin();
            if (incoming->getPrice() < level_it->first) break;

            auto& queue = level_it->second;
            while (incoming->getQuantity() > 0 && !queue.empty()) {
                auto& resting = queue.front();
                uint32_t traded_qty = std::min(incoming->getQuantity(), resting->getQuantity());

                trades.push_back(Trade{incoming->getId(), resting->getId(), level_it->first, traded_qty});

                incoming->fill(traded_qty);
                resting->fill(traded_qty);

                if (resting->getQuantity() == 0)
                    queue.erase(queue.begin());
            }
            if (queue.empty())
                asks_.erase(level_it);

        } else {
            if (bids_.empty()) break;
            auto level_it = bids_.begin();
            if (incoming->getPrice() > level_it->first) break;

            auto& queue = level_it->second;
            while (incoming->getQuantity() > 0 && !queue.empty()) {
                auto& resting = queue.front();
                uint32_t traded_qty = std::min(incoming->getQuantity(), resting->getQuantity());

                trades.push_back(Trade{resting->getId(), incoming->getId(), level_it->first, traded_qty});

                incoming->fill(traded_qty);
                resting->fill(traded_qty);

                if (resting->getQuantity() == 0)
                    queue.erase(queue.begin());
            }
            if (queue.empty())
                bids_.erase(level_it);
        }
    }

    if (incoming->getQuantity() > 0)
        add_order(std::move(incoming));

    return trades;
}
