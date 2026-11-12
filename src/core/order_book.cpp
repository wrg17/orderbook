//
// Created by Will George on 3/22/26.
//

#include <orderbook/core/order_book.hpp>

#include <cassert>
#include <ranges>

OrderBook::OrderBook() noexcept = default;

OrderBook::~OrderBook() noexcept {
    for (auto& order_ptr : orders_ | std::views::values) {
        order_ptr->unlink();
    }
}

std::optional<OrderBook::Front> OrderBook::getBestAsk() const noexcept {
    return front(asks_);
}

std::optional<OrderBook::Front> OrderBook::getBestBid() const noexcept {
    return front(bids_);
}

void OrderBook::takeBestAsk(Quantity qty) noexcept {
    assert(!asks_.empty()); // GCOVR_EXCL_BR_LINE

    take(*asks_.begin()->second.getFront(), qty);
}

void OrderBook::takeBestBid(Quantity qty) noexcept {
    assert(!bids_.empty()); // GCOVR_EXCL_BR_LINE

    take(*bids_.begin()->second.getFront(), qty);
}

std::optional<OrderBook::Front> OrderBook::front(const auto& map) const noexcept {
    if (map.empty()) {
        return std::nullopt;
    }

    auto it = map.begin();

    const Price kPrice = it->first;
    const PriceLevel* level = &it->second;

    const RestingOrder* const kLevelFront = level->getFront();

    return Front{
        .price = kPrice, .quantity = kLevelFront->getQuantity(), .id = kLevelFront->getId()};
}

void OrderBook::removeFromLevel(auto& map, RestingOrder& order) noexcept {
    const PriceLevel* const kLevelPtr = order.getLevel();

    const Price kPrice = kLevelPtr->getPrice();

    order.unlink();

    if (kLevelPtr->empty()) {
        map.erase(kPrice);
    }
}

void OrderBook::removeOrder(RestingOrder& order) noexcept {
    const OrderId kId = order.getId();

    if (order.getSide() == Side::BUY) {
        removeFromLevel(bids_, order);
    } else {
        removeFromLevel(asks_, order);
    }

    orders_.erase(kId);
}

void OrderBook::take(RestingOrder& order, Quantity qty) noexcept {
    const Quantity kRestingQty = order.getQuantity();

    assert(kRestingQty >= qty); // GCOVR_EXCL_BR_LINE

    order.reduce(qty);

    if (kRestingQty == qty) {
        removeOrder(order);
    }
}

void OrderBook::add(const Order& order) noexcept {
    add(order.getId(), order.getSide(), order.getPrice(), order.getQuantity());
}

void OrderBook::add(OrderId id, Side side, Price price, Quantity quantity) noexcept {
    auto order_ptr = std::make_unique<RestingOrder>(id, quantity, side);

    auto [it, _] =
        side == Side::BUY ? bids_.try_emplace(price, price) : asks_.try_emplace(price, price);
    it->second.pushBack(*order_ptr);

    orders_.emplace(id, std::move(order_ptr));
}

bool OrderBook::cancel(OrderId id) noexcept {
    const auto kOrderIt = orders_.find(id);
    if (kOrderIt == orders_.end()) {
        return false;
    }

    removeOrder(*kOrderIt->second);
    return true;
}

bool OrderBook::reduce(OrderId id, Quantity reduction) noexcept {
    const auto kOrderIt = orders_.find(id);
    if (kOrderIt == orders_.end()) {
        return false;
    }

    RestingOrder& order = *kOrderIt->second;

    if (reduction > order.getQuantity()) {
        return false;
    }

    take(order, reduction);
    return true;
}