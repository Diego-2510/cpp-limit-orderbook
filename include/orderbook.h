#pragma once

#include <map>
#include <vector>
#include <memory>
#include <optional>
#include "order.h"

class OrderBook {
private:
    // Bids: highest price first (std::greater)
    std::map<uint64_t, std::vector<std::unique_ptr<Order>>, std::greater<uint64_t>> bids_;
    
    // Asks: lowest price first (std::less, default)
    std::map<uint64_t, std::vector<std::unique_ptr<Order>>> asks_;

public:
    OrderBook() = default;
    
    void add_order(std::unique_ptr<Order> order);
    
    std::optional<uint64_t> get_best_bid() const;
    std::optional<uint64_t> get_best_ask() const;
    
    size_t get_bid_levels() const;
    size_t get_ask_levels() const;
};
