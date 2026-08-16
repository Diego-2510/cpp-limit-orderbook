#include "order.h"

#include <stdexcept>

Order::Order(uint32_t id, OrderType type, uint64_t price, uint32_t quantity)
    : id_(id),
      type_(type),
      price_(price),
      quantity_(quantity),
      timestamp_(std::chrono::system_clock::now()) {
    if (quantity == 0) {
        throw std::invalid_argument("order quantity must be greater than zero");
    }
}

uint32_t Order::getId() const noexcept { return id_; }

OrderType Order::getType() const noexcept { return type_; }

uint64_t Order::getPrice() const noexcept { return price_; }

uint32_t Order::getQuantity() const noexcept { return quantity_; }

std::chrono::system_clock::time_point Order::getTimestamp() const noexcept { return timestamp_; }

void Order::fill(uint32_t qty) {
    if (qty > quantity_) {
        throw std::invalid_argument("fill quantity exceeds remaining order quantity");
    }

    quantity_ -= qty;
}