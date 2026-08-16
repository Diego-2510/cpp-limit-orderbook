#pragma once

#include <cstddef>
#include <cstdint>
#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <vector>

#include "order.h"
#include "trade.h"

class OrderBook {
   public:
    OrderBook() = default;

    void add_order(std::unique_ptr<Order> order);

    [[nodiscard]] std::optional<uint64_t> get_best_bid() const;
    [[nodiscard]] std::optional<uint64_t> get_best_ask() const;

    [[nodiscard]] std::size_t get_bid_levels() const noexcept;
    [[nodiscard]] std::size_t get_ask_levels() const noexcept;

    std::vector<Trade> match_order(std::unique_ptr<Order> incoming);

   private:
    std::map<uint64_t, std::vector<std::unique_ptr<Order>>, std::greater<uint64_t>> bids_;

    std::map<uint64_t, std::vector<std::unique_ptr<Order>>> asks_;
};