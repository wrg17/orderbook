//
// Created by Will George on 3/22/26.
//

#pragma once

#include <atomic>
#include <compare>
#include <cstdint>

struct OrderId {
    std::uint64_t value;
    constexpr auto operator<=>(const OrderId&) const = default;
};

enum class Side : std::uint8_t { BUY, SELL };

struct Quantity {
    std::uint32_t value;
    constexpr auto operator<=>(const Quantity&) const = default;
};

struct Ticker {
    std::uint16_t value;
    constexpr auto operator<=>(const Ticker&) const = default;
};

// Stored in 1/1000 of a currency unit (1000 == $1.00).
struct Price {
    std::uint64_t value;
    constexpr auto operator<=>(const Price&) const = default;
};

class Order {
public:
    Order(Side side, Quantity quantity, Ticker ticker, Price price);
    ~Order() = default;

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
