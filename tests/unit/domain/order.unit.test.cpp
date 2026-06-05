//
// Created by Will George on 6/3/26.
//

#include <gtest/gtest.h>
#include <orderbook/domain/order.hpp>

constexpr Quantity kDefaultQuantity{100};
constexpr Ticker kDefaultTicker{1};
constexpr Price kDefaultPrice{1000};

TEST(Order, BasicConstructorBuyLimit) {
    const Order kOrder{Side::BUY, kDefaultQuantity, kDefaultTicker, kDefaultPrice};

    EXPECT_EQ(kOrder.getSide(), Side::BUY);
    EXPECT_EQ(kOrder.getQuantity(), kDefaultQuantity);
    EXPECT_EQ(kOrder.getTicker(), kDefaultTicker);
    EXPECT_EQ(kOrder.getPrice(), kDefaultPrice);
}

TEST(Order, BasicConstructorSellLimit) {
    const Order kOrder{Side::SELL, kDefaultQuantity, kDefaultTicker, kDefaultPrice};

    EXPECT_EQ(kOrder.getSide(), Side::SELL);
    EXPECT_EQ(kOrder.getQuantity(), kDefaultQuantity);
    EXPECT_EQ(kOrder.getTicker(), kDefaultTicker);
    EXPECT_EQ(kOrder.getPrice(), kDefaultPrice);
}

TEST(Order, IdsAreUniqueAcrossMultipleOrders) {
    const Order kA{Side::BUY, kDefaultQuantity, kDefaultTicker, kDefaultPrice};
    const Order kB{Side::SELL, kDefaultQuantity, kDefaultTicker, kDefaultPrice};
    const Order kC{Side::BUY, kDefaultQuantity, kDefaultTicker, kDefaultPrice};

    EXPECT_NE(kA.getId(), kB.getId());
    EXPECT_NE(kB.getId(), kC.getId());
    EXPECT_NE(kA.getId(), kC.getId());
}
