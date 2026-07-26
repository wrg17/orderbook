#include <gtest/gtest.h>
#include <orderbook/core/exchange.hpp>

constexpr Ticker kTicker{1};
constexpr Ticker kOtherTicker{2};
constexpr Price kPrice{100};
constexpr Quantity kQty{10};

TEST(Exchange, ListNewMarketReturnsTrue) {
    Exchange exchange{};

    EXPECT_TRUE(exchange.list(kTicker));
    EXPECT_TRUE(exchange.isListed(kTicker));
}

TEST(Exchange, ListExistingMarketReturnsFalse) {
    Exchange exchange{};

    ASSERT_TRUE(exchange.list(kTicker));
    EXPECT_FALSE(exchange.list(kTicker));
}

TEST(Exchange, UnlistedTickerIsNotListed) {
    const Exchange kExchange{};

    EXPECT_FALSE(kExchange.isListed(kTicker));
}

TEST(Exchange, SubmitToUnlistedTickerReturnsNullopt) {
    Exchange exchange{};

    const std::optional<std::vector<Trade>> kResult =
        exchange.submit(Order{Side::BUY, kQty, kTicker, kPrice});

    EXPECT_EQ(kResult, std::nullopt);
}

TEST(Exchange, SubmitToListedTickerRestsWithoutTrade) {
    Exchange exchange{};
    exchange.list(kTicker);

    const std::optional<std::vector<Trade>> kResult =
        exchange.submit(Order{Side::BUY, kQty, kTicker, kPrice});

    ASSERT_TRUE(kResult.has_value());
    EXPECT_TRUE(kResult.value_or(std::vector<Trade>{}).empty());
}

TEST(Exchange, SubmitRoutesCrossingOrdersToTheSameBook) {
    Exchange exchange{};
    exchange.list(kTicker);
    const Order kMaker{Side::SELL, kQty, kTicker, kPrice};

    ASSERT_TRUE(exchange.submit(kMaker).has_value());
    const std::optional<std::vector<Trade>> kResult =
        exchange.submit(Order{Side::BUY, kQty, kTicker, kPrice});

    ASSERT_TRUE(kResult.has_value());
    const std::vector<Trade> kTrades = kResult.value_or(std::vector<Trade>{});
    ASSERT_EQ(kTrades.size(), 1U);
    EXPECT_EQ(kTrades[0].getMakerOrderId(), kMaker.getId());
}

TEST(Exchange, SubmitRoutesByTickerToDistinctBooks) {
    Exchange exchange{};
    exchange.list(kTicker);
    exchange.list(kOtherTicker);

    ASSERT_TRUE(exchange.submit(Order{Side::SELL, kQty, kTicker, kPrice}).has_value());
    const std::optional<std::vector<Trade>> kResult =
        exchange.submit(Order{Side::BUY, kQty, kOtherTicker, kPrice});

    ASSERT_TRUE(kResult.has_value());
    EXPECT_TRUE(kResult.value_or(std::vector<Trade>{}).empty());
}

TEST(Exchange, CancelOnUnlistedTickerReturnsNullopt) {
    Exchange exchange{};

    EXPECT_EQ(exchange.cancel(kTicker, OrderId{1}), std::nullopt);
}

TEST(Exchange, CancelRoutesToBook) {
    Exchange exchange{};
    exchange.list(kTicker);
    const Order kResting{Side::SELL, kQty, kTicker, kPrice};
    ASSERT_TRUE(exchange.submit(kResting).has_value());

    const std::optional<bool> kCancelled = exchange.cancel(kTicker, kResting.getId());
    ASSERT_TRUE(kCancelled.has_value());
    EXPECT_TRUE(kCancelled.value_or(false));

    const std::optional<std::vector<Trade>> kResult =
        exchange.submit(Order{Side::BUY, kQty, kTicker, kPrice});
    ASSERT_TRUE(kResult.has_value());
    EXPECT_TRUE(kResult.value_or(std::vector<Trade>{}).empty());
}
