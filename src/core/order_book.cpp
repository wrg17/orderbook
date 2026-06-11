//
// Created by Will George on 3/22/26.
//

#include <orderbook/core/order_book.hpp>
#include <ranges>

OrderBook::OrderBook(Ticker ticker) noexcept : ticker_(ticker) {}

OrderBook::~OrderBook() noexcept {
    for (auto& order_ptr : orders_ | std::views::values) {
        if (PriceLevel* level = order_ptr->getLevel(); level != nullptr) {
            level->erase(*order_ptr);
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

    auto order_ptr = std::make_unique<RestingOrder>(kId, kQty);

    PriceLevel* level = order.getSide() == Side::BUY ? &bids_[kPrice] : &asks_[kPrice];
    level->pushBack(*order_ptr);

    orders_.emplace(kId, std::move(order_ptr));
}
