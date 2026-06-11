//
// Created by Will George on 3/22/26.
//

#pragma once

#include <map>
#include <memory>
#include <optional>
#include <orderbook/core/price_level.hpp>
#include <orderbook/domain/order.hpp>
#include <unordered_map>

class OrderBook {
public:
    explicit OrderBook(Ticker ticker) noexcept;

    ~OrderBook() noexcept;

    Ticker getTicker() const noexcept;

    void add(const Order& order) noexcept;

    std::optional<Price> getBestBid() const noexcept;

    std::optional<Price> getBestAsk() const noexcept;

private:
    Ticker ticker_{};

    std::map<Price, PriceLevel> asks_;
    std::map<Price, PriceLevel, std::greater<>> bids_;

    std::unordered_map<OrderId, std::unique_ptr<RestingOrder>> orders_;

    std::uint64_t trades_count_ = 0;
};
