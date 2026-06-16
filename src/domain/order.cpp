//
// Created by Will George on 3/22/26.
//

#include <cstdlib>
#include <format>
#include <orderbook/domain/order.hpp>

Order::Order(Side side, Quantity quantity, Ticker ticker, Price price) noexcept
    : id_(OrderId{++id_seq}), side_(side), quantity_(quantity), ticker_(ticker), price_(price) {}

OrderId Order::getId() const noexcept {
    return id_;
}

Side Order::getSide() const noexcept {
    return side_;
}

Quantity Order::getQuantity() const noexcept {
    return quantity_;
}

Ticker Order::getTicker() const noexcept {
    return ticker_;
}

Price Order::getPrice() const noexcept {
    return price_;
}

std::ostream& operator<<(std::ostream& os, const Order& order) noexcept {
    try {
        return os << std::format("{}", order);
    } catch (...) {
        std::abort();
    }
}

std::format_context::iterator
std::formatter<OrderId>::format(OrderId id, std::format_context& ctx) const noexcept {
    try {
        return std::formatter<std::uint64_t>::format(id.value, ctx);
    } catch (...) {
        std::abort();
    }
}

std::format_context::iterator
std::formatter<Side>::format(Side s, std::format_context& ctx) const noexcept {
    try {
        return std::formatter<std::string_view>::format(s == Side::BUY ? "BUY" : "SELL", ctx);
    } catch (...) {
        std::abort();
    }
}

std::format_context::iterator
std::formatter<Quantity>::format(Quantity quantity, std::format_context& ctx) const noexcept {
    try {
        return std::formatter<std::uint32_t>::format(quantity.value, ctx);
    } catch (...) {
        std::abort();
    }
}

std::format_context::iterator
std::formatter<Ticker>::format(Ticker ticker, std::format_context& ctx) const noexcept {
    try {
        return std::formatter<std::uint16_t>::format(ticker.value, ctx);
    } catch (...) {
        std::abort();
    }
}

std::format_context::iterator
std::formatter<Price>::format(Price price, std::format_context& ctx) const noexcept {
    try {
        return std::formatter<std::uint64_t>::format(price.value, ctx);
    } catch (...) {
        std::abort();
    }
}

std::format_context::iterator std::formatter<Order>::format(const Order& order,
                                                            std::format_context& ctx) noexcept {
    try {
        return std::format_to(ctx.out(), "Order(Id:{},Side:{},Quantity:{},Ticker:{},Price:{})",
                              order.getId(), order.getSide(), order.getQuantity(),
                              order.getTicker(), order.getPrice());
    } catch (...) {
        std::abort();
    }
}
