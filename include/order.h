#pragma once

#include <cstdint>
#include <chrono>

enum class OrderType { BUY, SELL };

class Order {
private:
    uint32_t id_;
    OrderType type_;
    uint64_t price_;  // in cents for precision
    uint32_t quantity_;
    std::chrono::system_clock::time_point timestamp_;

public:
    Order(uint32_t id, OrderType type, uint64_t price, 
          uint32_t quantity);
    
    uint32_t getId() const;
    OrderType getType() const;
    uint64_t getPrice() const;
    uint32_t getQuantity() const;
    std::chrono::system_clock::time_point getTimestamp() const;
};
