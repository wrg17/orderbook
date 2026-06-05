//
// Created by Will George on 6/4/26.
//

#include <format>
#include <gtest/gtest.h>
#include <orderbook/domain/trade.hpp>
#include <sstream>

constexpr OrderId kMakerOrderId{1};
constexpr OrderId kTakerOrderId{2};
constexpr auto kTakerSide = Side::BUY;
constexpr Ticker kTicker{555};
constexpr Price kPrice{10000};
constexpr Quantity kQuantity{1};
constexpr std::chrono::system_clock::time_point kTime{};

TEST(Trade, BasicConstructor) {
    const Trade trade{kMakerOrderId, kTakerOrderId, kTakerSide, kTicker, kPrice, kQuantity, kTime};

    EXPECT_NE(trade.getId(), TradeId{});
    EXPECT_EQ(trade.getMakerOrderId(), kMakerOrderId);
    EXPECT_EQ(trade.getTakerOrderId(), kTakerOrderId);
    EXPECT_EQ(trade.getTakerSide(), kTakerSide);
    EXPECT_EQ(trade.getTicker(), kTicker);
    EXPECT_EQ(trade.getPrice(), kPrice);
    EXPECT_EQ(trade.getQuantity(), kQuantity);
    EXPECT_EQ(trade.getTime().time_since_epoch().count(), kTime.time_since_epoch().count());
}

TEST(Trade, IdsAreUniqueAcrossMultipleTrades) {
    const Trade a{kMakerOrderId, kTakerOrderId, kTakerSide, kTicker, kPrice, kQuantity, kTime};
    const Trade b{kMakerOrderId, kTakerOrderId, kTakerSide, kTicker, kPrice, kQuantity, kTime};
    const Trade c{kMakerOrderId, kTakerOrderId, kTakerSide, kTicker, kPrice, kQuantity, kTime};

    EXPECT_NE(a.getId(), b.getId());
    EXPECT_NE(b.getId(), c.getId());
    EXPECT_NE(a.getId(), c.getId());
}

TEST(Trade, BasicOStreamBuySide) {
    const Trade trade{kMakerOrderId, kTakerOrderId, Side::BUY, kTicker, kPrice, kQuantity, kTime};
    std::stringstream ss;
    ss << trade;

    const auto expected = std::format(
        "Trade:{},Maker:{},Taker:{},Side:{},Ticker:{},Price:{},Quantity:{},Time:{}",
        trade.getId().value, kMakerOrderId.value, kTakerOrderId.value, "BUY", kTicker.value,
        kPrice.value, kQuantity.value, kTime.time_since_epoch().count());
    EXPECT_EQ(ss.str(), expected);
}

TEST(Trade, BasicOStreamSellSide) {
    const Trade trade{kMakerOrderId, kTakerOrderId, Side::SELL, kTicker, kPrice, kQuantity, kTime};
    std::stringstream ss;
    ss << trade;

    const auto expected = std::format(
        "Trade:{},Maker:{},Taker:{},Side:{},Ticker:{},Price:{},Quantity:{},Time:{}",
        trade.getId().value, kMakerOrderId.value, kTakerOrderId.value, "SELL", kTicker.value,
        kPrice.value, kQuantity.value, kTime.time_since_epoch().count());
    EXPECT_EQ(ss.str(), expected);
}
