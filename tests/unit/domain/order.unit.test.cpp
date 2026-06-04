//
// Created by Will George on 6/3/26.
//

#include <gtest/gtest.h>
#include <orderbook/domain/order.hpp>

constexpr Quantity kDefaultQuantity{100};
constexpr Ticker kDefaultTicker{1};
constexpr Price kDefaultPrice{1000};

TEST(Order, BasicConstructorBuyLimit) {
    const Order order{Order::Side::BUY, kDefaultQuantity, kDefaultTicker, kDefaultPrice};

    EXPECT_EQ(order.getSide(), Order::Side::BUY);
    EXPECT_EQ(order.getQuantity(), kDefaultQuantity);
    EXPECT_EQ(order.getTicker(), kDefaultTicker);
    EXPECT_EQ(order.getPrice(), kDefaultPrice);
}

TEST(Order, BasicConstructorSellLimit) {
    const Order order{Order::Side::SELL, kDefaultQuantity, kDefaultTicker, kDefaultPrice};

    EXPECT_EQ(order.getSide(), Order::Side::SELL);
    EXPECT_EQ(order.getQuantity(), kDefaultQuantity);
    EXPECT_EQ(order.getTicker(), kDefaultTicker);
    EXPECT_EQ(order.getPrice(), kDefaultPrice);
}

TEST(Order, IdsAreUniqueAcrossMultipleOrders) {
    const Order a{Order::Side::BUY, kDefaultQuantity, kDefaultTicker, kDefaultPrice};
    const Order b{Order::Side::SELL, kDefaultQuantity, kDefaultTicker, kDefaultPrice};
    const Order c{Order::Side::BUY, kDefaultQuantity, kDefaultTicker, kDefaultPrice};

    EXPECT_NE(a.getId(), b.getId());
    EXPECT_NE(b.getId(), c.getId());
    EXPECT_NE(a.getId(), c.getId());
}
