//
// Created by Will George on 3/22/26.
//

#include <gtest/gtest.h>
#include <orderbook/core/order_book.hpp>

constexpr Ticker kTicker{1};
constexpr Price kMatchPrice{1'0000};

const Order kBuy{Side::BUY, Quantity{1}, kTicker, kMatchPrice};
const Order kSell{Side::SELL, Quantity{1}, kTicker, kMatchPrice};
const Order kUntracked{Side::BUY, Quantity{1}, kTicker, kMatchPrice};

TEST(OrderBook, ConstructorBasic) {
    const OrderBook kOrderBook{kTicker};

    EXPECT_EQ(kOrderBook.getTicker(), kTicker);
}

TEST(OrderBook, GetBestBidWhenEmpty) {
    const OrderBook kOrderBook{kTicker};

    EXPECT_EQ(kOrderBook.getBestBid(), std::nullopt);
}

TEST(OrderBook, GetBestAskWhenEmpty) {
    const OrderBook kOrderBook{kTicker};

    EXPECT_EQ(kOrderBook.getBestAsk(), std::nullopt);
}

TEST(OrderBook, AddBasicOrdersToBids) {
    OrderBook order_book{kTicker};
    order_book.add(kBuy);

    EXPECT_EQ(order_book.getBestAsk(), std::nullopt);
    EXPECT_EQ(order_book.getBestBid(), kMatchPrice);
}

TEST(OrderBook, AddBasicOrdersToAsks) {
    OrderBook order_book{kTicker};
    order_book.add(kSell);

    EXPECT_EQ(order_book.getBestAsk(), kMatchPrice);
    EXPECT_EQ(order_book.getBestBid(), std::nullopt);
}

TEST(OrderBook, CancelRemovesOrderFromBook) {
    OrderBook order_book{kTicker};
    order_book.add(kBuy);
    ASSERT_EQ(order_book.getBestBid(), kMatchPrice);

    order_book.cancel(kBuy.getId());

    EXPECT_EQ(order_book.getBestBid(), std::nullopt);
}

TEST(OrderBook, CancelUnknownIdIsNoOp) {
    OrderBook order_book{kTicker};
    order_book.add(kBuy);
    ASSERT_EQ(order_book.getBestBid(), kMatchPrice);

    order_book.cancel(kUntracked.getId());

    EXPECT_EQ(order_book.getBestBid(), kMatchPrice);
}