//
// Created by Will George on 3/22/26.
//

#include <orderbook/core/order_book.hpp>
#include <ranges>

OrderBook::OrderBook(Ticker ticker) noexcept : ticker_(ticker) {}

OrderBook::~OrderBook() noexcept {
    for (auto& order_ptr : orders_ | std::views::values) {
        if (order_ptr->getLevel() != nullptr) {
            order_ptr->unlink();
        }
    }
}

Ticker OrderBook::getTicker() const noexcept {
    return ticker_;
}

std::optional<Price> OrderBook::getBestAsk() const noexcept {
    if (asks_.empty()) {
        return std::nullopt;
    }

    const auto kMin = asks_.begin();
    auto price = kMin->first;
    return {price};
}

std::optional<Price> OrderBook::getBestBid() const noexcept {
    if (bids_.empty()) {
        return std::nullopt;
    }

    const auto kMax = bids_.begin();
    auto price = kMax->first;
    return {price};
}

void OrderBook::add(const Order& order) noexcept {
    const OrderId kId = order.getId();
    const Price kPrice = order.getPrice();
    const Quantity kQty = order.getQuantity();
    const Side kSide = order.getSide();

    auto order_ptr = std::make_unique<RestingOrder>(kId, kQty, kSide);

    auto [it, _] =
        kSide == Side::BUY ? bids_.try_emplace(kPrice, kPrice) : asks_.try_emplace(kPrice, kPrice);
    it->second.pushBack(*order_ptr);

    orders_.emplace(kId, std::move(order_ptr));
}

void OrderBook::cancel(const OrderId kId) noexcept {
    auto node = orders_.extract(kId);
    if (node.empty()) {
        return;
    }

    RestingOrder* order = node.mapped().get();

    PriceLevel* level = order->getLevel();
    const Price kPrice = level->getPrice();
    const Side kSide = order->getSide();

    order->unlink();

    if (level->empty()) {
        if (kSide == Side::BUY) {
            bids_.erase(kPrice);
        } else {
            asks_.erase(kPrice);
        }
    }
}