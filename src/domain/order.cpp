//
// Created by Will George on 3/22/26.
//

#include <orderbook/domain/order.hpp>

Order::Order(const Side side, const Quantity quantity, const Ticker ticker, const Price price)
    : id_(++count_), side_(side), quantity_(quantity), ticker_(ticker), price_(price) {}

std::uint64_t Order::getId() const { return id_; }

Order::Side Order::getSide() const { return side_; }

Quantity Order::getQuantity() const { return quantity_; }

Ticker Order::getTicker() const { return ticker_; }

Price Order::getPrice() const { return price_; }
