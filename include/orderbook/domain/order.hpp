//
// Created by Will George on 3/22/26.
//

#pragma once

#include <atomic>
#include <compare>
#include <cstdint>
#include <format>
#include <iosfwd>
#include <string_view>

struct OrderId {
    std::uint64_t value;
    constexpr std::strong_ordering operator<=>(const OrderId&) const = default;
};

enum class Side : std::uint8_t { BUY, SELL };

struct Quantity {
    std::uint32_t value;

    constexpr std::strong_ordering operator<=>(const Quantity&) const = default;

    constexpr Quantity& operator-=(Quantity other) {
        value -= other.value;
        return *this;
    }

    constexpr Quantity& operator+=(Quantity other) {
        value += other.value;
        return *this;
    }
};

constexpr Quantity operator-(Quantity lhs, Quantity rhs) {
    return lhs -= rhs;
}

constexpr Quantity operator+(Quantity lhs, Quantity rhs) {
    return lhs += rhs;
}

struct Ticker {
    std::uint16_t value;
    constexpr std::strong_ordering operator<=>(const Ticker&) const = default;
};

// Stored in 1/1000 of a currency unit (1000 == $1.00).
struct Price {
    std::uint64_t value;
    constexpr std::strong_ordering operator<=>(const Price&) const = default;
};

class Order {
public:
    Order(Side side, Quantity quantity, Ticker ticker, Price price);

    [[nodiscard]] OrderId getId() const;
    [[nodiscard]] Side getSide() const;
    [[nodiscard]] Quantity getQuantity() const;
    [[nodiscard]] Ticker getTicker() const;
    [[nodiscard]] Price getPrice() const;

private:
    inline static std::atomic<std::uint64_t> id_seq = 0;

    OrderId id_;
    Side side_;
    Quantity quantity_;
    Ticker ticker_;
    Price price_;
};

std::ostream& operator<<(std::ostream& os, const Order& order);

template <> struct std::formatter<OrderId> : std::formatter<std::uint64_t> {
    std::format_context::iterator format(OrderId id, std::format_context& ctx) const;
};

template <> struct std::formatter<Side> : std::formatter<std::string_view> {
    std::format_context::iterator format(Side s, std::format_context& ctx) const;
};

template <> struct std::formatter<Quantity> : std::formatter<std::uint32_t> {
    std::format_context::iterator format(Quantity quantity, std::format_context& ctx) const;
};

template <> struct std::formatter<Ticker> : std::formatter<std::uint16_t> {
    std::format_context::iterator format(Ticker ticker, std::format_context& ctx) const;
};

template <> struct std::formatter<Price> : std::formatter<std::uint64_t> {
    std::format_context::iterator format(Price price, std::format_context& ctx) const;
};

template <> struct std::formatter<Order> {
    static constexpr std::format_parse_context::const_iterator
    parse(const std::format_parse_context& ctx) {
        return ctx.begin();
    }
    static std::format_context::iterator format(const Order& order, std::format_context& ctx);
};
