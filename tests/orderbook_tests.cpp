#include <gtest/gtest.h>

#include <memory>
#include <stdexcept>

#include "order.h"
#include "orderbook.h"

TEST(OrderBookTest, EmptyBookHasNoBestPrices) {
    const OrderBook book;

    EXPECT_FALSE(
        book.get_best_bid().has_value());

    EXPECT_FALSE(
        book.get_best_ask().has_value());

    EXPECT_EQ(
        book.get_bid_levels(),
        0U);

    EXPECT_EQ(
        book.get_ask_levels(),
        0U);
}

TEST(OrderBookTest, RejectsNullOrders) {
    OrderBook book;

    EXPECT_THROW(
        book.add_order(nullptr),
        std::invalid_argument);

    EXPECT_THROW(
        book.match_order(nullptr),
        std::invalid_argument);
}

TEST(
    OrderBookTest,
    TracksBestBidAndAskAcrossPriceLevels) {

    OrderBook book;

    book.add_order(
        std::make_unique<Order>(
            1,
            OrderType::BUY,
            10000,
            10));

    book.add_order(
        std::make_unique<Order>(
            2,
            OrderType::BUY,
            10100,
            10));

    book.add_order(
        std::make_unique<Order>(
            3,
            OrderType::SELL,
            10300,
            10));

    book.add_order(
        std::make_unique<Order>(
            4,
            OrderType::SELL,
            10200,
            10));

    ASSERT_TRUE(
        book.get_best_bid().has_value());

    ASSERT_TRUE(
        book.get_best_ask().has_value());

    EXPECT_EQ(
        *book.get_best_bid(),
        10100U);

    EXPECT_EQ(
        *book.get_best_ask(),
        10200U);

    EXPECT_EQ(
        book.get_bid_levels(),
        2U);

    EXPECT_EQ(
        book.get_ask_levels(),
        2U);
}

TEST(
    OrderBookTest,
    FullyMatchesAtRestingOrderPrice) {

    OrderBook book;

    book.add_order(
        std::make_unique<Order>(
            10,
            OrderType::SELL,
            10000,
            100));

    const auto trades =
        book.match_order(
            std::make_unique<Order>(
                11,
                OrderType::BUY,
                10100,
                100));

    ASSERT_EQ(
        trades.size(),
        1U);

    EXPECT_EQ(
        trades[0].buy_order_id,
        11U);

    EXPECT_EQ(
        trades[0].sell_order_id,
        10U);

    EXPECT_EQ(
        trades[0].price,
        10000U);

    EXPECT_EQ(
        trades[0].quantity,
        100U);

    EXPECT_EQ(
        book.get_ask_levels(),
        0U);

    EXPECT_EQ(
        book.get_bid_levels(),
        0U);
}

TEST(
    OrderBookTest,
    IncomingRemainderRestsOnBookAfterPartialFill) {

    OrderBook book;

    book.add_order(
        std::make_unique<Order>(
            20,
            OrderType::SELL,
            10000,
            60));

    const auto trades =
        book.match_order(
            std::make_unique<Order>(
                21,
                OrderType::BUY,
                10000,
                100));

    ASSERT_EQ(
        trades.size(),
        1U);

    EXPECT_EQ(
        trades[0].quantity,
        60U);

    EXPECT_EQ(
        book.get_ask_levels(),
        0U);

    ASSERT_TRUE(
        book.get_best_bid().has_value());

    EXPECT_EQ(
        *book.get_best_bid(),
        10000U);

    const auto closing_trades =
        book.match_order(
            std::make_unique<Order>(
                22,
                OrderType::SELL,
                10000,
                100));

    ASSERT_EQ(
        closing_trades.size(),
        1U);

    EXPECT_EQ(
        closing_trades[0].quantity,
        40U);
}

TEST(
    OrderBookTest,
    RestingRemainderStaysOnBookAfterPartialFill) {

    OrderBook book;

    book.add_order(
        std::make_unique<Order>(
            30,
            OrderType::SELL,
            10000,
            100));

    const auto first_trades =
        book.match_order(
            std::make_unique<Order>(
                31,
                OrderType::BUY,
                10000,
                40));

    ASSERT_EQ(
        first_trades.size(),
        1U);

    EXPECT_EQ(
        first_trades[0].quantity,
        40U);

    EXPECT_EQ(
        book.get_ask_levels(),
        1U);

    const auto second_trades =
        book.match_order(
            std::make_unique<Order>(
                32,
                OrderType::BUY,
                10000,
                100));

    ASSERT_EQ(
        second_trades.size(),
        1U);

    EXPECT_EQ(
        second_trades[0].quantity,
        60U);

    EXPECT_EQ(
        book.get_ask_levels(),
        0U);

    ASSERT_TRUE(
        book.get_best_bid().has_value());

    EXPECT_EQ(
        *book.get_best_bid(),
        10000U);
}

TEST(
    OrderBookTest,
    NonCrossingOrderRestsWithoutTrade) {

    OrderBook book;

    book.add_order(
        std::make_unique<Order>(
            40,
            OrderType::SELL,
            10500,
            100));

    const auto trades =
        book.match_order(
            std::make_unique<Order>(
                41,
                OrderType::BUY,
                10000,
                100));

    EXPECT_TRUE(trades.empty());

    EXPECT_EQ(
        book.get_bid_levels(),
        1U);

    EXPECT_EQ(
        book.get_ask_levels(),
        1U);

    EXPECT_EQ(
        *book.get_best_bid(),
        10000U);

    EXPECT_EQ(
        *book.get_best_ask(),
        10500U);
}

TEST(
    OrderBookTest,
    PreservesFifoWithinSamePriceLevel) {

    OrderBook book;

    book.add_order(
        std::make_unique<Order>(
            50,
            OrderType::SELL,
            10000,
            30));

    book.add_order(
        std::make_unique<Order>(
            51,
            OrderType::SELL,
            10000,
            40));

    const auto trades =
        book.match_order(
            std::make_unique<Order>(
                52,
                OrderType::BUY,
                10000,
                50));

    ASSERT_EQ(
        trades.size(),
        2U);

    EXPECT_EQ(
        trades[0].sell_order_id,
        50U);

    EXPECT_EQ(
        trades[0].quantity,
        30U);

    EXPECT_EQ(
        trades[1].sell_order_id,
        51U);

    EXPECT_EQ(
        trades[1].quantity,
        20U);
}

TEST(
    OrderBookTest,
    SweepsBestPricesBeforeWorsePrices) {

    OrderBook book;

    book.add_order(
        std::make_unique<Order>(
            60,
            OrderType::SELL,
            10000,
            20));

    book.add_order(
        std::make_unique<Order>(
            61,
            OrderType::SELL,
            10100,
            20));

    book.add_order(
        std::make_unique<Order>(
            62,
            OrderType::SELL,
            10200,
            20));

    const auto trades =
        book.match_order(
            std::make_unique<Order>(
                63,
                OrderType::BUY,
                10200,
                50));

    ASSERT_EQ(
        trades.size(),
        3U);

    EXPECT_EQ(
        trades[0].price,
        10000U);

    EXPECT_EQ(
        trades[1].price,
        10100U);

    EXPECT_EQ(
        trades[2].price,
        10200U);

    EXPECT_EQ(
        trades[0].quantity,
        20U);

    EXPECT_EQ(
        trades[1].quantity,
        20U);

    EXPECT_EQ(
        trades[2].quantity,
        10U);

    ASSERT_TRUE(
        book.get_best_ask().has_value());

    EXPECT_EQ(
        *book.get_best_ask(),
        10200U);
}

TEST(
    OrderBookTest,
    SellOrderMatchesHighestBidFirst) {

    OrderBook book;

    book.add_order(
        std::make_unique<Order>(
            70,
            OrderType::BUY,
            10000,
            20));

    book.add_order(
        std::make_unique<Order>(
            71,
            OrderType::BUY,
            10200,
            20));

    book.add_order(
        std::make_unique<Order>(
            72,
            OrderType::BUY,
            10100,
            20));

    const auto trades =
        book.match_order(
            std::make_unique<Order>(
                73,
                OrderType::SELL,
                10000,
                30));

    ASSERT_EQ(
        trades.size(),
        2U);

    EXPECT_EQ(
        trades[0].buy_order_id,
        71U);

    EXPECT_EQ(
        trades[0].price,
        10200U);

    EXPECT_EQ(
        trades[0].quantity,
        20U);

    EXPECT_EQ(
        trades[1].buy_order_id,
        72U);

    EXPECT_EQ(
        trades[1].price,
        10100U);

    EXPECT_EQ(
        trades[1].quantity,
        10U);
}