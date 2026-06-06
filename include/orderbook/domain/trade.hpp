//
// Created by Will George on 3/22/26.
//

#pragma once

#include <atomic>
#include <chrono>
#include <compare>
#include <format>
#include <iosfwd>
#include <orderbook/domain/order.hpp>

struct TradeId {
    std::uint64_t value;
    constexpr std::strong_ordering operator<=>(const TradeId&) const = default;
};

class Trade {
public:
    Trade(OrderId maker, OrderId taker, Side side, Ticker ticker, Price price, Quantity quantity,
          std::chrono::system_clock::time_point time);

    [[nodiscard]] TradeId getId() const;

    [[nodiscard]] OrderId getMakerOrderId() const;

    [[nodiscard]] OrderId getTakerOrderId() const;

    [[nodiscard]] Side getTakerSide() const;

    [[nodiscard]] Ticker getTicker() const;

    [[nodiscard]] Price getPrice() const;

    [[nodiscard]] Quantity getQuantity() const;

    [[nodiscard]] std::chrono::system_clock::time_point getTime() const;

private:
    inline static std::atomic<std::uint64_t> id_seq = 0;

    TradeId id_;

    OrderId maker_order_id_;
    OrderId taker_order_id_;

    Side taker_side_;

    Ticker ticker_;
    Price price_;
    Quantity quantity_;

    std::chrono::system_clock::time_point time_;
};

std::ostream& operator<<(std::ostream& os, const Trade& trade);

template <> struct std::formatter<TradeId> : std::formatter<std::uint64_t> {
    std::format_context::iterator format(TradeId id, std::format_context& ctx) const;
};

template <> struct std::formatter<Trade> {
    static constexpr auto parse(const std::format_parse_context& ctx) {
        return ctx.begin();
    }
    static std::format_context::iterator format(const Trade& trade, std::format_context& ctx);
};
