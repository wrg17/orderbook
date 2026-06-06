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
    const Trade kTrade{kMakerOrderId, kTakerOrderId, kTakerSide, kTicker, kPrice, kQuantity, kTime};

    EXPECT_NE(kTrade.getId(), TradeId{});
    EXPECT_EQ(kTrade.getMakerOrderId(), kMakerOrderId);
    EXPECT_EQ(kTrade.getTakerOrderId(), kTakerOrderId);
    EXPECT_EQ(kTrade.getTakerSide(), kTakerSide);
    EXPECT_EQ(kTrade.getTicker(), kTicker);
    EXPECT_EQ(kTrade.getPrice(), kPrice);
    EXPECT_EQ(kTrade.getQuantity(), kQuantity);
    EXPECT_EQ(kTrade.getTime().time_since_epoch().count(), kTime.time_since_epoch().count());
}

TEST(Trade, IdsAreUniqueAcrossMultipleTrades) {
    const Trade kA{kMakerOrderId, kTakerOrderId, kTakerSide, kTicker, kPrice, kQuantity, kTime};
    const Trade kB{kMakerOrderId, kTakerOrderId, kTakerSide, kTicker, kPrice, kQuantity, kTime};
    const Trade kC{kMakerOrderId, kTakerOrderId, kTakerSide, kTicker, kPrice, kQuantity, kTime};

    EXPECT_NE(kA.getId(), kB.getId());
    EXPECT_NE(kB.getId(), kC.getId());
    EXPECT_NE(kA.getId(), kC.getId());
}

TEST(Trade, BasicOStreamBuySide) {
    const Trade kTrade{kMakerOrderId, kTakerOrderId, Side::BUY, kTicker, kPrice, kQuantity, kTime};
    std::stringstream ss;
    ss << kTrade;

    const auto kExpected =
        std::format("Trade(Id:{},Maker:{},Taker:{},Side:{},Ticker:{},Price:{},Quantity:{},Time:{})",
                    kTrade.getId(), kMakerOrderId.value, kTakerOrderId.value, "BUY", kTicker.value,
                    kPrice.value, kQuantity.value, kTime.time_since_epoch().count());
    EXPECT_EQ(ss.str(), kExpected);
}

TEST(Trade, BasicOStreamSellSide) {
    const Trade kTrade{kMakerOrderId, kTakerOrderId, Side::SELL, kTicker, kPrice, kQuantity, kTime};
    std::stringstream ss;
    ss << kTrade;

    const auto kExpected =
        std::format("Trade(Id:{},Maker:{},Taker:{},Side:{},Ticker:{},Price:{},Quantity:{},Time:{})",
                    kTrade.getId(), kMakerOrderId.value, kTakerOrderId.value, "SELL", kTicker.value,
                    kPrice.value, kQuantity.value, kTime.time_since_epoch().count());
    EXPECT_EQ(ss.str(), kExpected);
}

TEST(TradeId, FormatsValue) {
    EXPECT_EQ(std::format("{}", TradeId{42}), "42");
}

TEST(Trade, FormatsAllFields) {
    const Trade kTrade{kMakerOrderId, kTakerOrderId, Side::BUY, kTicker, kPrice, kQuantity, kTime};

    const auto kExpected = std::format(
        "Trade(Id:{},Maker:{},Taker:{},Side:BUY,Ticker:{},Price:{},Quantity:{},Time:{})",
        kTrade.getId().value, kMakerOrderId.value, kTakerOrderId.value, kTicker.value, kPrice.value,
        kQuantity.value, kTime.time_since_epoch().count());
    EXPECT_EQ(std::format("{}", kTrade), kExpected);
}
