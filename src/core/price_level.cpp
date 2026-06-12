//
// Created by Will George on 6/8/26.
//

#include "orderbook/core/price_level.hpp"
#include <cassert>

/*
 * PriceLevel
 */

PriceLevel::PriceLevel() noexcept
    : order_count_(0), total_quantity_(Quantity{}), sentinel_(OrderId{}, Quantity{}) {
    sentinel_.level_ = this;
    sentinel_.next_ = &sentinel_;
    sentinel_.prev_ = &sentinel_;
}

PriceLevel::~PriceLevel() noexcept {
    assert(sentinel_.next_ == &sentinel_ && sentinel_.prev_ == &sentinel_);
    sentinel_.level_ = nullptr;
    sentinel_.prev_ = nullptr;
    sentinel_.next_ = nullptr;
}

bool PriceLevel::empty() const noexcept {
    return order_count_ == 0;
}

RestingOrder* PriceLevel::getFront() const noexcept {
    return sentinel_.next_ == &sentinel_ ? nullptr : sentinel_.next_;
}

Quantity PriceLevel::getTotalQuantity() const noexcept {
    return total_quantity_;
}

std::size_t PriceLevel::getOrderCount() const noexcept {
    return order_count_;
}

void PriceLevel::pushBack(RestingOrder& order) noexcept {
    assert(order.level_ == nullptr);
    assert(order.next_ == nullptr);
    assert(order.prev_ == nullptr);

    order.level_ = this;
    order.prev_ = sentinel_.prev_;
    order.next_ = &sentinel_;

    sentinel_.prev_->next_ = &order;
    sentinel_.prev_ = &order;

    ++order_count_;
    total_quantity_ += order.getQuantity();
}

/*
 * RestingOrder
 */

RestingOrder::RestingOrder(OrderId id, Quantity quantity) noexcept : id_(id), quantity_(quantity) {}

RestingOrder::~RestingOrder() noexcept {
    assert(level_ == nullptr && next_ == nullptr && prev_ == nullptr);
}

OrderId RestingOrder::getId() const noexcept {
    return id_;
}

Quantity RestingOrder::getQuantity() const noexcept {
    return quantity_;
}

PriceLevel* RestingOrder::getLevel() const noexcept {
    return level_;
}

RestingOrder* RestingOrder::getNext() const noexcept {
    if (level_ == nullptr || next_ == &level_->sentinel_) {
        return nullptr;
    }
    return next_;
}

RestingOrder* RestingOrder::getPrev() const noexcept {
    if (level_ == nullptr || prev_ == &level_->sentinel_) {
        return nullptr;
    }
    return prev_;
}

void RestingOrder::unlink() noexcept {
    assert(level_ != nullptr);
    assert(prev_ != nullptr);
    assert(next_ != nullptr);

    // prev <-> this <-> next
    prev_->next_ = next_;
    next_->prev_ = prev_;

    --level_->order_count_;
    level_->total_quantity_ -= quantity_;

    next_ = nullptr;
    prev_ = nullptr;
    level_ = nullptr;
}

void RestingOrder::reduce(Quantity reduction) noexcept {
    assert(reduction <= quantity_);
    assert(level_ != nullptr);

    quantity_ -= reduction;
    level_->total_quantity_ -= reduction;
}