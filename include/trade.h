#pragma once

#include <cstdint>

struct Trade {
    uint32_t buy_order_id;
    uint32_t sell_order_id;
    uint64_t price;
    uint32_t quantity;
};
