//
// Created by Will George on 6/7/26.
//

#include <gtest/gtest.h>
#include <orderbook/core/price_level.hpp>

constexpr OrderId kFirstOrder{1};
constexpr OrderId kSecondOrder{2};
constexpr OrderId kThirdOrder{3};

constexpr Quantity kQuantity{10};
constexpr Quantity kReduction{3};
constexpr Quantity kZeroQuantity{};

constexpr Price kPrice{1'0000};
constexpr Side kSide{Side::BUY};

constexpr std::size_t kZeroOrders = 0;
constexpr std::size_t kOneOrder = 1;
constexpr std::size_t kTwoOrders = 2;
constexpr std::size_t kThreeOrders = 3;

TEST(RestingOrder, ConstructorInitializesFields) {
    const RestingOrder kRestingOrder{kFirstOrder, kQuantity, kSide};

    EXPECT_EQ(kRestingOrder.getId(), kFirstOrder);
    EXPECT_EQ(kRestingOrder.getQuantity(), kQuantity);
    EXPECT_EQ(kRestingOrder.getSide(), kSide);
    EXPECT_EQ(kRestingOrder.getLevel(), nullptr);
    EXPECT_EQ(kRestingOrder.getNext(), nullptr);
    EXPECT_EQ(kRestingOrder.getPrev(), nullptr);
}

class PriceLevelTest : public ::testing::Test {
protected:
    void TearDown() override {
        if (first_order_.getLevel() != nullptr) {
            first_order_.unlink();
        }

        if (second_order_.getLevel() != nullptr) {
            second_order_.unlink();
        }

        if (third_order_.getLevel() != nullptr) {
            third_order_.unlink();
        }
    }

    PriceLevel level_{kPrice};
    RestingOrder first_order_{kFirstOrder, kQuantity, kSide};
    RestingOrder second_order_{kSecondOrder, kQuantity, kSide};
    RestingOrder third_order_{kThirdOrder, kQuantity, kSide};
};

TEST(PriceLevel, ConstructorInitializesEmpty) {
    const PriceLevel kLevel{kPrice};

    EXPECT_TRUE(kLevel.empty());
    EXPECT_EQ(kLevel.getPrice(), kPrice);
    EXPECT_EQ(kLevel.getOrderCount(), kZeroOrders);
    EXPECT_EQ(kLevel.getTotalQuantity(), kZeroQuantity);
    EXPECT_EQ(kLevel.getFront(), nullptr);
}

TEST_F(PriceLevelTest, PushBackUpdatesState) {
    ASSERT_TRUE(level_.empty());

    level_.pushBack(first_order_);
    EXPECT_FALSE(level_.empty());
    EXPECT_EQ(level_.getFront(), &first_order_);
    EXPECT_EQ(level_.getOrderCount(), kOneOrder);
    EXPECT_EQ(level_.getTotalQuantity(), kQuantity);
}

TEST_F(PriceLevelTest, PushBackSecondOrderKeepsFirstAsFront) {
    ASSERT_TRUE(level_.empty());

    level_.pushBack(first_order_);
    ASSERT_EQ(level_.getFront(), &first_order_);

    level_.pushBack(second_order_);
    EXPECT_EQ(level_.getFront(), &first_order_);
}

TEST_F(PriceLevelTest, PushBackSecondOrderUpdatesState) {
    ASSERT_TRUE(level_.empty());

    level_.pushBack(first_order_);
    level_.pushBack(second_order_);

    EXPECT_EQ(level_.getOrderCount(), kTwoOrders);
    EXPECT_EQ(level_.getTotalQuantity(), 2U * kQuantity);
}

TEST_F(PriceLevelTest, PushBackLinksOrderToLevel) {
    ASSERT_TRUE(level_.empty());

    level_.pushBack(first_order_);
    ASSERT_EQ(first_order_.getLevel(), &level_);
}

TEST_F(PriceLevelTest, PushBackIncrementsOrderCount) {
    ASSERT_TRUE(level_.empty());

    level_.pushBack(first_order_);
    ASSERT_EQ(level_.getOrderCount(), kOneOrder);

    level_.pushBack(second_order_);
    ASSERT_EQ(level_.getOrderCount(), kTwoOrders);

    level_.pushBack(third_order_);
    ASSERT_EQ(level_.getOrderCount(), kThreeOrders);
}

TEST_F(PriceLevelTest, UnlinkOnlyOrderRestoresEmpty) {
    ASSERT_TRUE(level_.empty());

    level_.pushBack(first_order_);
    ASSERT_FALSE(level_.empty());

    first_order_.unlink();
    ASSERT_TRUE(level_.empty());
}

TEST_F(PriceLevelTest, UnlinkFromFrontSequentially) {
    ASSERT_TRUE(level_.empty());
    ASSERT_EQ(level_.getOrderCount(), kZeroOrders);
    ASSERT_EQ(level_.getTotalQuantity(), kZeroQuantity);
    ASSERT_EQ(level_.getFront(), nullptr);

    level_.pushBack(first_order_);
    ASSERT_EQ(level_.getOrderCount(), kOneOrder);
    ASSERT_EQ(level_.getTotalQuantity(), kQuantity);
    ASSERT_EQ(level_.getFront(), &first_order_);

    level_.pushBack(second_order_);
    ASSERT_EQ(level_.getOrderCount(), kTwoOrders);
    ASSERT_EQ(level_.getTotalQuantity(), 2U * kQuantity);
    ASSERT_EQ(level_.getFront(), &first_order_);

    level_.pushBack(third_order_);
    ASSERT_EQ(level_.getOrderCount(), kThreeOrders);
    ASSERT_EQ(level_.getTotalQuantity(), 3U * kQuantity);
    ASSERT_EQ(level_.getFront(), &first_order_);

    first_order_.unlink();
    ASSERT_EQ(level_.getOrderCount(), kTwoOrders);
    ASSERT_EQ(level_.getTotalQuantity(), 2U * kQuantity);
    ASSERT_EQ(level_.getFront(), &second_order_);

    second_order_.unlink();
    ASSERT_EQ(level_.getOrderCount(), kOneOrder);
    ASSERT_EQ(level_.getTotalQuantity(), kQuantity);
    ASSERT_EQ(level_.getFront(), &third_order_);

    third_order_.unlink();
    ASSERT_EQ(level_.getOrderCount(), kZeroOrders);
    ASSERT_EQ(level_.getTotalQuantity(), kZeroQuantity);
    ASSERT_EQ(level_.getFront(), nullptr);
}

TEST_F(PriceLevelTest, UnlinkMiddleKeepsListWalkable) {
    ASSERT_TRUE(level_.empty());

    level_.pushBack(first_order_);
    ASSERT_EQ(level_.getOrderCount(), kOneOrder);
    ASSERT_EQ(level_.getFront(), &first_order_);

    level_.pushBack(second_order_);
    ASSERT_EQ(level_.getOrderCount(), kTwoOrders);
    ASSERT_EQ(level_.getFront(), &first_order_);

    level_.pushBack(third_order_);
    ASSERT_EQ(level_.getOrderCount(), kThreeOrders);
    ASSERT_EQ(level_.getFront(), &first_order_);

    second_order_.unlink();
    ASSERT_EQ(level_.getOrderCount(), kTwoOrders);
    ASSERT_EQ(level_.getFront(), &first_order_);

    first_order_.unlink();
    ASSERT_EQ(level_.getOrderCount(), kOneOrder);
    ASSERT_EQ(level_.getFront(), &third_order_);

    third_order_.unlink();
    ASSERT_EQ(level_.getOrderCount(), kZeroOrders);
    ASSERT_EQ(level_.getFront(), nullptr);
}

TEST_F(PriceLevelTest, PushBackSoleOrderHasNoNeighbors) {
    level_.pushBack(first_order_);

    EXPECT_EQ(first_order_.getNext(), nullptr);
    EXPECT_EQ(first_order_.getPrev(), nullptr);
}

TEST_F(PriceLevelTest, PushBackLinksNextAndPrev) {
    level_.pushBack(first_order_);
    level_.pushBack(second_order_);
    level_.pushBack(third_order_);

    EXPECT_EQ(first_order_.getPrev(), nullptr);
    EXPECT_EQ(first_order_.getNext(), &second_order_);

    EXPECT_EQ(second_order_.getPrev(), &first_order_);
    EXPECT_EQ(second_order_.getNext(), &third_order_);

    EXPECT_EQ(third_order_.getPrev(), &second_order_);
    EXPECT_EQ(third_order_.getNext(), nullptr);
}

TEST_F(PriceLevelTest, UnlinkMiddleSplicesFrontAndBack) {
    level_.pushBack(first_order_);
    level_.pushBack(second_order_);
    level_.pushBack(third_order_);

    second_order_.unlink();

    EXPECT_EQ(first_order_.getNext(), &third_order_);
    EXPECT_EQ(third_order_.getPrev(), &first_order_);
}

TEST_F(PriceLevelTest, UnlinkNullsOrderLevel) {
    level_.pushBack(first_order_);
    ASSERT_EQ(first_order_.getLevel(), &level_);

    first_order_.unlink();
    ASSERT_EQ(first_order_.getLevel(), nullptr);
}

TEST_F(PriceLevelTest, UnlinkNullsOrderNeighbors) {
    level_.pushBack(first_order_);
    level_.pushBack(second_order_);
    level_.pushBack(third_order_);

    second_order_.unlink();

    EXPECT_EQ(second_order_.getNext(), nullptr);
    EXPECT_EQ(second_order_.getPrev(), nullptr);
}

TEST_F(PriceLevelTest, ReduceDecrementsOrderQuantity) {
    level_.pushBack(first_order_);
    ASSERT_EQ(level_.getTotalQuantity(), kQuantity);

    first_order_.reduce(kReduction);
    ASSERT_EQ(first_order_.getQuantity(), kQuantity - kReduction);
}

TEST_F(PriceLevelTest, ReduceUpdatesLevelTotal) {
    level_.pushBack(first_order_);
    ASSERT_EQ(level_.getTotalQuantity(), kQuantity);

    first_order_.reduce(kReduction);
    ASSERT_EQ(level_.getTotalQuantity(), kQuantity - kReduction);
}

TEST_F(PriceLevelTest, ReduceThenUnlinkLeavesTotalAtZero) {
    level_.pushBack(first_order_);
    ASSERT_EQ(level_.getTotalQuantity(), kQuantity);

    first_order_.reduce(kReduction);
    ASSERT_EQ(level_.getTotalQuantity(), kQuantity - kReduction);

    first_order_.unlink();
    ASSERT_EQ(level_.getTotalQuantity(), kZeroQuantity);
}

#ifndef NDEBUG

TEST(PriceLevelDeathTest, DestructorAbortsWhenNotEmpty) {
    EXPECT_DEATH(
        {
            RestingOrder order(kFirstOrder, kQuantity, kSide);
            {
                PriceLevel level{kPrice};
                level.pushBack(order);
            }
        },
        "");
}

TEST(PriceLevelDeathTest, PushBackAbortsWhenOrderAlreadyLinked) {
    EXPECT_DEATH(
        {
            PriceLevel level{kPrice};
            RestingOrder order(kFirstOrder, kQuantity, kSide);
            level.pushBack(order);
            level.pushBack(order);
        },
        "");
}

TEST(RestingOrderDeathTest, DestructorAbortsWhenStillLinked) {
    EXPECT_DEATH(
        {
            PriceLevel level{kPrice};
            RestingOrder order(kFirstOrder, kQuantity, kSide);
            level.pushBack(order);
        },
        "");
}

TEST(RestingOrderDeathTest, UnlinkAbortsWhenNotLinked) {
    EXPECT_DEATH(
        {
            RestingOrder order(kFirstOrder, kQuantity, kSide);
            order.unlink();
        },
        "");
}

TEST(RestingOrderDeathTest, ReduceAbortsWhenReductionExceedsQuantity) {
    EXPECT_DEATH(
        {
            RestingOrder order(kFirstOrder, kQuantity, kSide);
            order.reduce(kQuantity + kReduction);
        },
        "");
}

TEST(RestingOrderDeathTest, ReduceAbortsWhenNotLinked) {
    EXPECT_DEATH(
        {
            RestingOrder order(kFirstOrder, kQuantity, kSide);
            order.reduce(kReduction);
        },
        "");
}

#endif // NDEBUG
