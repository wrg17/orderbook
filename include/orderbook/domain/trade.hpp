//
// Created by Will George on 3/22/26.
//

#pragma once

#include <atomic>
#include <chrono>
#include <compare>
#include <iosfwd>
#include <orderbook/domain/order.hpp>

struct TradeId {
    std::uint64_t value;
    constexpr auto operator<=>(const TradeId&) const = default;
};

class Trade {
public:
    Trade(OrderId maker, OrderId taker, Side side, Ticker ticker, Price price, Quantity quantity,
          std::chrono::system_clock::time_point time);
    ~Trade() = default;

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