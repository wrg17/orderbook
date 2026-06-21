#include <gtest/gtest.h>
#include <orderbook/core/matching_engine.hpp>

constexpr Ticker kTicker{1};

TEST(MatchingEngine, ConstructorInitTheBasics) {
    const MatchingEngine kMatchingEngine{kTicker};

    EXPECT_EQ(kMatchingEngine.getTicker(), kTicker);
}
