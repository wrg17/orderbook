//
// Created by Will George on 3/22/26.
//

#include <format>
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

std::ostream& operator<<(std::ostream& os, const Order& order) {
    return os << std::format("{}", order);
}

std::format_context::iterator std::formatter<OrderId>::format(OrderId id,
                                                              std::format_context& ctx) const {
    return std::formatter<std::uint64_t>::format(id.value, ctx);
}

std::format_context::iterator std::formatter<Side>::format(Side s, std::format_context& ctx) const {
    return std::formatter<std::string_view>::format(s == Side::BUY ? "BUY" : "SELL", ctx);
}

std::format_context::iterator std::formatter<Quantity>::format(Quantity quantity,
                                                               std::format_context& ctx) const {
    return std::formatter<std::uint32_t>::format(quantity.value, ctx);
}

std::format_context::iterator std::formatter<Ticker>::format(Ticker ticker,
                                                             std::format_context& ctx) const {
    return std::formatter<std::uint16_t>::format(ticker.value, ctx);
}

std::format_context::iterator std::formatter<Price>::format(Price price,
                                                            std::format_context& ctx) const {
    return std::formatter<std::uint64_t>::format(price.value, ctx);
}

std::format_context::iterator std::formatter<Order>::format(const Order& order,
                                                            std::format_context& ctx) {
    return std::format_to(ctx.out(), "Order(Id:{},Side:{},Quantity:{},Ticker:{},Price:{})",
                          order.getId(), order.getSide(), order.getQuantity(), order.getTicker(),
                          order.getPrice());
}
