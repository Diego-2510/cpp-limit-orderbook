#pragma once

#include <chrono>
#include <cstdint>

enum class OrderType { BUY, SELL };

class Order {
public:
    Order(uint32_t id, OrderType type, uint64_t price, uint32_t quantity);

    [[nodiscard]] uint32_t getId() const noexcept;
    [[nodiscard]] OrderType getType() const noexcept;
    [[nodiscard]] uint64_t getPrice() const noexcept;
    [[nodiscard]] uint32_t getQuantity() const noexcept;
    [[nodiscard]] std::chrono::system_clock::time_point getTimestamp() const noexcept;

    void fill(uint32_t qty);

private:
    uint32_t id_;
    OrderType type_;
    uint64_t price_;
    uint32_t quantity_;
    std::chrono::system_clock::time_point timestamp_;
};