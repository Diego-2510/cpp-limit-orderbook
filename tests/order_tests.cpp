#include <gtest/gtest.h>

#include <stdexcept>

#include "order.h"

TEST(OrderTest, StoresConstructorValues) {
    const Order order(
        7,
        OrderType::BUY,
        10125,
        50);

    EXPECT_EQ(order.getId(), 7U);
    EXPECT_EQ(
        order.getType(),
        OrderType::BUY);
    EXPECT_EQ(
        order.getPrice(),
        10125U);
    EXPECT_EQ(
        order.getQuantity(),
        50U);
}

TEST(OrderTest, RejectsZeroQuantity) {
    EXPECT_THROW(
        (Order{
            1,
            OrderType::BUY,
            10000,
            0}),
        std::invalid_argument);
}

TEST(OrderTest, FillReducesRemainingQuantity) {
    Order order(
        1,
        OrderType::SELL,
        10000,
        100);

    order.fill(40);

    EXPECT_EQ(
        order.getQuantity(),
        60U);
}

TEST(OrderTest, ExactFillLeavesZeroQuantity) {
    Order order(
        1,
        OrderType::SELL,
        10000,
        100);

    order.fill(100);

    EXPECT_EQ(
        order.getQuantity(),
        0U);
}

TEST(
    OrderTest,
    RejectsOverfillInsteadOfUnsignedUnderflow) {

    Order order(
        1,
        OrderType::SELL,
        10000,
        100);

    EXPECT_THROW(
        order.fill(101),
        std::invalid_argument);

    EXPECT_EQ(
        order.getQuantity(),
        100U);
}