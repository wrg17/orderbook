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

TEST(OrderBook, GetBestBidWhenEmpty) {
    const OrderBook kOrderBook{};

    EXPECT_EQ(kOrderBook.getBestBid(), std::nullopt);
}

TEST(OrderBook, GetBestAskWhenEmpty) {
    const OrderBook kOrderBook{};

    EXPECT_EQ(kOrderBook.getBestAsk(), std::nullopt);
}

TEST(OrderBook, AddBasicOrdersToBids) {
    OrderBook order_book{};
    order_book.add(kBuy);

    EXPECT_EQ(order_book.getBestAsk(), std::nullopt);
    EXPECT_EQ(order_book.getBestBid(),
              (OrderBook::Front{kMatchPrice, kBuy.getQuantity(), kBuy.getId()}));
}

TEST(OrderBook, AddBasicOrdersToAsks) {
    OrderBook order_book{};
    order_book.add(kSell);

    EXPECT_EQ(order_book.getBestAsk(),
              (OrderBook::Front{kMatchPrice, kSell.getQuantity(), kSell.getId()}));
    EXPECT_EQ(order_book.getBestBid(), std::nullopt);
}

TEST(OrderBook, AddTwoOrdersAndPopOnlyOne) {
    OrderBook order_book{};

    order_book.add(kBuy);
    order_book.add(kBigBuy);

    ASSERT_EQ(order_book.getBestBid(),
              (OrderBook::Front{kMatchPrice, kBuy.getQuantity(), kBuy.getId()}));

    order_book.takeBestBid(kBuy.getQuantity());

    ASSERT_EQ(order_book.getBestBid(),
              (OrderBook::Front{kMatchPrice, kBigBuy.getQuantity(), kBigBuy.getId()}));
}

TEST(OrderBook, CancelRemovesBuyFromBook) {
    OrderBook order_book{};
    order_book.add(kBuy);
    ASSERT_EQ(order_book.getBestBid(),
              (OrderBook::Front{kMatchPrice, kBuy.getQuantity(), kBuy.getId()}));

    order_book.cancel(kBuy.getId());

    EXPECT_EQ(order_book.getBestBid(), std::nullopt);
}

TEST(OrderBook, CancelRemovesSaleFromBook) {
    OrderBook order_book{};
    order_book.add(kSell);
    ASSERT_EQ(order_book.getBestAsk(),
              (OrderBook::Front{kMatchPrice, kSell.getQuantity(), kSell.getId()}));

    order_book.cancel(kSell.getId());

    EXPECT_EQ(order_book.getBestAsk(), std::nullopt);
}

TEST(OrderBook, CancelUnknownIdIsNoOp) {
    OrderBook order_book{};
    order_book.add(kBuy);
    const OrderBook::Front kExpected{
        .price = kMatchPrice, .quantity = kBuy.getQuantity(), .id = kBuy.getId()};
    ASSERT_EQ(order_book.getBestBid(), kExpected);

    order_book.cancel(kUntracked.getId());

    EXPECT_EQ(order_book.getBestBid(), kExpected);
}

TEST(OrderBook, TakeBestBidPartial) {
    OrderBook order_book{};
    order_book.add(kBigBuy);

    order_book.takeBestBid(kPartialQty);

    EXPECT_EQ(order_book.getBestBid(), (OrderBook::Front{.price = kMatchPrice,
                                                         .quantity = kBigQuantity - kPartialQty,
                                                         .id = kBigBuy.getId()}));
}

TEST(OrderBook, TakeBestBidFull) {
    OrderBook order_book{};
    order_book.add(kBigBuy);

    order_book.takeBestBid(kBigQuantity);

    EXPECT_EQ(order_book.getBestBid(), std::nullopt);
}

TEST(OrderBook, TakeBestAskPartial) {
    OrderBook order_book{};
    order_book.add(kBigSell);

    order_book.takeBestAsk(kPartialQty);

    EXPECT_EQ(order_book.getBestAsk(), (OrderBook::Front{.price = kMatchPrice,
                                                         .quantity = kBigQuantity - kPartialQty,
                                                         .id = kBigSell.getId()}));
}

TEST(OrderBook, TakeBestAskFull) {
    OrderBook order_book{};
    order_book.add(kBigSell);

    order_book.takeBestAsk(kBigQuantity);

    EXPECT_EQ(order_book.getBestAsk(), std::nullopt);
}

TEST(OrderBook, BookFrontComparison) {
    constexpr OrderId kId{1};
    constexpr OrderBook::Front kBase{.price = kMatchPrice, .quantity = kBigQuantity, .id = kId};

    constexpr OrderBook::Front kMatch{.price = kMatchPrice, .quantity = kBigQuantity, .id = kId};
    ASSERT_EQ(kBase, kMatch);

    constexpr OrderBook::Front kDiffPrice{
        .price = Price{kMatchPrice.value + 1}, .quantity = kBigQuantity, .id = kId};
    ASSERT_NE(kBase, kDiffPrice);

    constexpr OrderBook::Front kDiffQuantity{
        .price = kMatchPrice, .quantity = Quantity{kBigQuantity.value + 1}, .id = kId};
    ASSERT_NE(kBase, kDiffQuantity);

    constexpr OrderBook::Front kDiffId{
        .price = kMatchPrice, .quantity = kBigQuantity, .id = OrderId{kId.value + 1}};
    ASSERT_NE(kBase, kDiffId);
}

#ifndef NDEBUG
TEST(OrderBook, TakeFailsForAnEmptyBidMap) {
    OrderBook order_book{};
    ASSERT_DEATH(order_book.takeBestAsk(kBigQuantity), "");
}

TEST(OrderBook, TakeFailsWhenQuantityExceedsFront) {
    OrderBook order_book{};
    order_book.add(kBuy);
    ASSERT_DEATH(order_book.takeBestBid(kBigQuantity), "");
}
#endif