#include <gtest/gtest.h>
#include <orderbook/core/matching_engine.hpp>

constexpr Ticker kTicker{1};

TEST(MatchingEngine, ConstructorInitTheBasics) {
    const MatchingEngine kMatchingEngine{kTicker};

    EXPECT_EQ(kMatchingEngine.getTicker(), kTicker);
}

TEST(MatchingEngine, SubmitIntoEmptyBookRestsWithoutTrade) {
    MatchingEngine engine{kTicker};

    const std::vector<Trade> kTrades =
        engine.submit(Order{Side::BUY, Quantity{10}, kTicker, Price{100}});

    EXPECT_TRUE(kTrades.empty());
}

TEST(MatchingEngine, BuyCrossesRestingAskProducesTrade) {
    MatchingEngine engine{kTicker};
    const Order kMaker{Side::SELL, Quantity{10}, kTicker, Price{100}};
    const Order kTaker{Side::BUY, Quantity{10}, kTicker, Price{100}};

    EXPECT_TRUE(engine.submit(kMaker).empty());
    const std::vector<Trade> kTrades = engine.submit(kTaker);

    ASSERT_EQ(kTrades.size(), 1U);
    EXPECT_EQ(kTrades[0].getMakerOrderId(), kMaker.getId());
    EXPECT_EQ(kTrades[0].getTakerOrderId(), kTaker.getId());
    EXPECT_EQ(kTrades[0].getTakerSide(), Side::BUY);
    EXPECT_EQ(kTrades[0].getPrice(), Price{100});
    EXPECT_EQ(kTrades[0].getQuantity(), Quantity{10});
}

TEST(MatchingEngine, SellCrossesRestingBidProducesTrade) {
    MatchingEngine engine{kTicker};
    const Order kMaker{Side::BUY, Quantity{10}, kTicker, Price{100}};
    const Order kTaker{Side::SELL, Quantity{10}, kTicker, Price{100}};

    EXPECT_TRUE(engine.submit(kMaker).empty());
    const std::vector<Trade> kTrades = engine.submit(kTaker);

    ASSERT_EQ(kTrades.size(), 1U);
    EXPECT_EQ(kTrades[0].getTakerSide(), Side::SELL);
    EXPECT_EQ(kTrades[0].getPrice(), Price{100});
    EXPECT_EQ(kTrades[0].getQuantity(), Quantity{10});
}

TEST(MatchingEngine, BuyBelowAskDoesNotCross) {
    MatchingEngine engine{kTicker};

    EXPECT_TRUE(engine.submit(Order{Side::SELL, Quantity{10}, kTicker, Price{110}}).empty());
    const std::vector<Trade> kTrades =
        engine.submit(Order{Side::BUY, Quantity{10}, kTicker, Price{100}});

    EXPECT_TRUE(kTrades.empty());
}

TEST(MatchingEngine, SellAboveBidDoesNotCross) {
    MatchingEngine engine{kTicker};

    EXPECT_TRUE(engine.submit(Order{Side::BUY, Quantity{10}, kTicker, Price{100}}).empty());
    const std::vector<Trade> kTrades =
        engine.submit(Order{Side::SELL, Quantity{10}, kTicker, Price{110}});

    EXPECT_TRUE(kTrades.empty());
}

TEST(MatchingEngine, BuyConsumesMultipleRestingAsks) {
    MatchingEngine engine{kTicker};

    EXPECT_TRUE(engine.submit(Order{Side::SELL, Quantity{5}, kTicker, Price{100}}).empty());
    EXPECT_TRUE(engine.submit(Order{Side::SELL, Quantity{5}, kTicker, Price{100}}).empty());
    const std::vector<Trade> kTrades =
        engine.submit(Order{Side::BUY, Quantity{10}, kTicker, Price{100}});

    ASSERT_EQ(kTrades.size(), 2U);
    EXPECT_EQ(kTrades[0].getQuantity(), Quantity{5});
    EXPECT_EQ(kTrades[1].getQuantity(), Quantity{5});
}

TEST(MatchingEngine, BuyPartiallyConsumesRestingAsk) {
    MatchingEngine engine{kTicker};

    EXPECT_TRUE(engine.submit(Order{Side::SELL, Quantity{10}, kTicker, Price{100}}).empty());
    const std::vector<Trade> kTrades =
        engine.submit(Order{Side::BUY, Quantity{4}, kTicker, Price{100}});

    ASSERT_EQ(kTrades.size(), 1U);
    EXPECT_EQ(kTrades[0].getQuantity(), Quantity{4});
}

TEST(MatchingEngine, PartialFillRestsResidualQuantity) {
    MatchingEngine engine{kTicker};

    // A 4-lot ask, then a 10-lot buy: 4 fills, 6 must rest as a bid.
    EXPECT_TRUE(engine.submit(Order{Side::SELL, Quantity{4}, kTicker, Price{100}}).empty());
    const std::vector<Trade> kFirst =
        engine.submit(Order{Side::BUY, Quantity{10}, kTicker, Price{100}});
    ASSERT_EQ(kFirst.size(), 1U);
    EXPECT_EQ(kFirst[0].getQuantity(), Quantity{4});

    // A 10-lot sell hits the rested bid: it must trade only the residual 6, not 10.
    const std::vector<Trade> kSecond =
        engine.submit(Order{Side::SELL, Quantity{10}, kTicker, Price{100}});
    ASSERT_EQ(kSecond.size(), 1U);
    EXPECT_EQ(kSecond[0].getQuantity(), Quantity{6});
}