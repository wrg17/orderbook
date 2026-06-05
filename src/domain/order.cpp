//
// Created by Will George on 3/22/26.
//

#include <orderbook/domain/order.hpp>

Order::Order(Side side, Quantity quantity, Ticker ticker, Price price)
    : id_(OrderId{++id_seq}), side_(side), quantity_(quantity), ticker_(ticker), price_(price) {}

OrderId Order::getId() const {
    return id_;
}

Side Order::getSide() const {
    return side_;
}

Quantity Order::getQuantity() const {
    return quantity_;
}

Ticker Order::getTicker() const {
    return ticker_;
}

Price Order::getPrice() const {
    return price_;
}
