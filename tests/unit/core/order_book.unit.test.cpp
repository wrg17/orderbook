//
// Created by Will George on 3/22/26.
//

#include <gtest/gtest.h>
#include <orderbook/core/order_book.hpp>

constexpr Ticker kTicker{1};
constexpr Price kMatchPrice{1'0000};
constexpr Quantity kBigQuantity{10};
constexpr Quantity kPartialQty{3};

const Order kBuy{Side::BUY, Quantity{1}, kTicker, kMatchPrice};
const Order kSell{Side::SELL, Quantity{1}, kTicker, kMatchPrice};
const Order kUntracked{Side::BUY, Quantity{1}, kTicker, kMatchPrice};
const Order kBigBuy{Side::BUY, kBigQuantity, kTicker, kMatchPrice};
const Order kBigSell{Side::SELL, kBigQuantity, kTicker, kMatchPrice};

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
    EXPECT_EQ(order_book.getBestBid(),
              (OrderBook::Front{kMatchPrice, kBuy.getQuantity(), kBuy.getId()}));
}

TEST(OrderBook, AddBasicOrdersToAsks) {
    OrderBook order_book{kTicker};
    order_book.add(kSell);

    EXPECT_EQ(order_book.getBestAsk(),
              (OrderBook::Front{kMatchPrice, kSell.getQuantity(), kSell.getId()}));
    EXPECT_EQ(order_book.getBestBid(), std::nullopt);
}

TEST(OrderBook, CancelRemovesOrderFromBook) {
    OrderBook order_book{kTicker};
    order_book.add(kBuy);
    ASSERT_EQ(order_book.getBestBid(),
              (OrderBook::Front{kMatchPrice, kBuy.getQuantity(), kBuy.getId()}));

    order_book.cancel(kBuy.getId());

    EXPECT_EQ(order_book.getBestBid(), std::nullopt);
}

TEST(OrderBook, CancelUnknownIdIsNoOp) {
    OrderBook order_book{kTicker};
    order_book.add(kBuy);
    const OrderBook::Front kExpected{
        .price = kMatchPrice, .quantity = kBuy.getQuantity(), .id = kBuy.getId()};
    ASSERT_EQ(order_book.getBestBid(), kExpected);

    order_book.cancel(kUntracked.getId());

    EXPECT_EQ(order_book.getBestBid(), kExpected);
}

TEST(OrderBook, TakeBestBidPartial) {
    OrderBook order_book{kTicker};
    order_book.add(kBigBuy);

    order_book.takeBestBid(kPartialQty);

    EXPECT_EQ(order_book.getBestBid(), (OrderBook::Front{.price = kMatchPrice,
                                                         .quantity = kBigQuantity - kPartialQty,
                                                         .id = kBigBuy.getId()}));
}

TEST(OrderBook, TakeBestBidFull) {
    OrderBook order_book{kTicker};
    order_book.add(kBigBuy);

    order_book.takeBestBid(kBigQuantity);

    EXPECT_EQ(order_book.getBestBid(), std::nullopt);
}

TEST(OrderBook, TakeBestAskPartial) {
    OrderBook order_book{kTicker};
    order_book.add(kBigSell);

    order_book.takeBestAsk(kPartialQty);

    EXPECT_EQ(order_book.getBestAsk(), (OrderBook::Front{.price = kMatchPrice,
                                                         .quantity = kBigQuantity - kPartialQty,
                                                         .id = kBigSell.getId()}));
}

TEST(OrderBook, TakeBestAskFull) {
    OrderBook order_book{kTicker};
    order_book.add(kBigSell);

    order_book.takeBestAsk(kBigQuantity);

    EXPECT_EQ(order_book.getBestAsk(), std::nullopt);
}