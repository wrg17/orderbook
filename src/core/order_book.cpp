//
// Created by Will George on 3/22/26.
//

#include <orderbook/core/order_book.hpp>

#include <cassert>
#include <ranges>

OrderBook::OrderBook(Ticker ticker) noexcept : ticker_(ticker) {}

OrderBook::~OrderBook() noexcept {
    for (auto& order_ptr : orders_ | std::views::values) {
        order_ptr->unlink();
    }
}

Ticker OrderBook::getTicker() const noexcept {
    return ticker_;
}

std::optional<OrderBook::Front> OrderBook::getBestAsk() const noexcept {
    return front(asks_);
}

std::optional<OrderBook::Front> OrderBook::getBestBid() const noexcept {
    return front(bids_);
}

void OrderBook::takeBestAsk(Quantity qty) noexcept {
    take(asks_, qty);
}

void OrderBook::takeBestBid(Quantity qty) noexcept {
    take(bids_, qty);
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

void OrderBook::removeOrder(auto& map, RestingOrder& order) noexcept {
    const PriceLevel* const kLevelPtr = order.getLevel();

    const Price kPrice = kLevelPtr->getPrice();

    order.unlink();

    if (kLevelPtr->empty()) {
        map.erase(kPrice);
    }
}

void OrderBook::take(auto& map, Quantity qty) noexcept {
    assert(!map.empty()); // GCOVR_EXCL_BR_LINE

    auto it = map.begin();

    const PriceLevel* const kLevel = &it->second;

    RestingOrder* const kRestingPtr = kLevel->getFront();

    const Quantity kRestingQty = kRestingPtr->getQuantity();
    const OrderId kId = kRestingPtr->getId();

    assert(kRestingQty >= qty); // GCOVR_EXCL_BR_LINE

    kRestingPtr->reduce(qty);

    if (kRestingQty == qty) {
        removeOrder(map, *kRestingPtr);
        orders_.erase(kId);
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

void OrderBook::cancel(OrderId id) noexcept {
    const auto kNode = orders_.extract(id);
    if (kNode.empty()) {
        return;
    }

    std::unique_ptr<RestingOrder> order = std::move(kNode.mapped());

    if (order->getSide() == Side::BUY) {
        removeOrder(bids_, *order);
    } else {
        removeOrder(asks_, *order);
    }
}