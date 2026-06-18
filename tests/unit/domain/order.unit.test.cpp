//
// Created by Will George on 6/3/26.
//

#include <format>
#include <gtest/gtest.h>
#include <orderbook/domain/order.hpp>
#include <sstream>

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

TEST(Price, Comparisons) {
    constexpr Price kLow{1};
    constexpr Price kHigh{2};
    EXPECT_LT(kLow, kHigh);
    EXPECT_GT(kHigh, kLow);

    EXPECT_LE(kLow, kLow);
    EXPECT_GE(kHigh, kHigh);

    EXPECT_EQ(kHigh, kHigh);
    EXPECT_EQ(kLow, kLow);
}

TEST(OrderId, FormatsValue) {
    EXPECT_EQ(std::format("{}", OrderId{42}), "42");
}

TEST(Side, FormatsBuy) {
    EXPECT_EQ(std::format("{}", Side::BUY), "BUY");
}

TEST(Side, FormatsSell) {
    EXPECT_EQ(std::format("{}", Side::SELL), "SELL");
}

TEST(Quantity, FormatsValue) {
    EXPECT_EQ(std::format("{}", Quantity{100}), "100");
}

constexpr Quantity kQuantityProduct{30};
constexpr std::uint32_t kScalar = 3;

constexpr Quantity kQuantityLhs{10};
constexpr Quantity kQuantityRhs{3};
constexpr Quantity kQuantityDifference{7};
constexpr Quantity kQuantitySum{13};

TEST(Quantity, MinusEqualsSubtracts) {
    Quantity q = kQuantityLhs;
    q -= kQuantityRhs;
    EXPECT_EQ(q, kQuantityDifference);
}

TEST(Quantity, PlusEqualsAdds) {
    Quantity q = kQuantityLhs;
    q += kQuantityRhs;
    EXPECT_EQ(q, kQuantitySum);
}

TEST(Quantity, MinusReturnsDifference) {
    EXPECT_EQ(kQuantityLhs - kQuantityRhs, kQuantityDifference);
}

TEST(Quantity, PlusReturnsSum) {
    EXPECT_EQ(kQuantityLhs + kQuantityRhs, kQuantitySum);
}

TEST(Quantity, StarEqualsMultiplies) {
    Quantity q = kQuantityLhs;
    q *= kScalar;
    EXPECT_EQ(q, kQuantityProduct);
}
TEST(Quantity, ScalarTimesQuantityReturnsProduct) {
    EXPECT_EQ(kScalar * kQuantityLhs, kQuantityProduct);
}
TEST(Quantity, QuantityTimesScalarReturnsProduct) {
    EXPECT_EQ(kQuantityLhs * kScalar, kQuantityProduct);
}

TEST(Ticker, FormatsValue) {
    EXPECT_EQ(std::format("{}", Ticker{555}), "555");
}

TEST(Price, FormatsValue) {
    EXPECT_EQ(std::format("{}", Price{10000}), "10000");
}

TEST(Order, FormatsAllFields) {
    const Order kOrder{Side::BUY, kDefaultQuantity, kDefaultTicker, kDefaultPrice};

    const auto kExpected =
        std::format("Order(Id:{},Side:BUY,Quantity:{},Ticker:{},Price:{})", kOrder.getId().value,
                    kDefaultQuantity.value, kDefaultTicker.value, kDefaultPrice.value);
    EXPECT_EQ(std::format("{}", kOrder), kExpected);
}

TEST(Order, OStream) {
    const Order kOrder{Side::SELL, kDefaultQuantity, kDefaultTicker, kDefaultPrice};
    std::stringstream ss;
    ss << kOrder;

    EXPECT_EQ(ss.str(), std::format("{}", kOrder));
}
