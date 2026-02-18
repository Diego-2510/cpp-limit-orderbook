#include "order.h"

Order::Order(uint32_t id, OrderType type, uint64_t price,
             uint32_t quantity)
    : id_(id), type_(type), price_(price),
      quantity_(quantity),
      timestamp_(std::chrono::system_clock::now()) {
}

uint32_t Order::getId() const {
    return id_;
}

OrderType Order::getType() const {
    return type_;
}

uint64_t Order::getPrice() const {
    return price_;
}

uint32_t Order::getQuantity() const {
    return quantity_;
}

std::chrono::system_clock::time_point Order::getTimestamp() const {
    return timestamp_;
}

void Order::fill(uint32_t qty) {
    quantity_ -= qty;
}
