//
// Created by Will George on 6/8/26.
//

#include "orderbook/core/price_level.hpp"
#include <cassert>

/*
 * PriceLevel
 */

bool PriceLevel::empty() const noexcept {
    return order_count_ == 0;
}

RestingOrder* PriceLevel::getFront() const noexcept {
    return front_;
}

Quantity PriceLevel::getTotalQuantity() const noexcept {
    return total_quantity_;
}

std::size_t PriceLevel::getOrderCount() const noexcept {
    return order_count_;
}

void PriceLevel::pushBack(RestingOrder& order) noexcept {
    assert(order.level_ == nullptr && order.next_ == nullptr && order.prev_ == nullptr);

    if (front_ == nullptr) {
        front_ = &order;
        back_ = &order;
    } else {
        back_->next_ = &order;
        order.prev_ = back_;
        back_ = &order;
    }

    ++order_count_;
    total_quantity_ += order.getQuantity();
    order.level_ = this;
}

void PriceLevel::erase(RestingOrder& order) noexcept {
    assert(order.level_ == this);

    --order_count_;
    total_quantity_ -= order.getQuantity();

    // prev <-> order <-> next
    RestingOrder* prev = order.prev_;
    RestingOrder* next = order.next_;

    if (prev != nullptr) {
        prev->next_ = next;
    }
    if (next != nullptr) {
        next->prev_ = prev;
    }

    if (front_ == &order) {
        front_ = next;
    }
    if (back_ == &order) {
        back_ = prev;
    }
    order.level_ = nullptr;
    order.next_ = nullptr;
    order.prev_ = nullptr;
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
    return next_;
}

RestingOrder* RestingOrder::getPrev() const noexcept {
    return prev_;
}

void RestingOrder::reduce(Quantity reduction) noexcept {
    assert(reduction <= quantity_);
    assert(level_ != nullptr);

    quantity_ -= reduction;
    level_->total_quantity_ -= reduction;
}