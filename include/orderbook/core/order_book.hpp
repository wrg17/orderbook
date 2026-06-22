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
    struct Front {
        Price price;
        Quantity quantity;
        OrderId id;

        bool operator==(const Front&) const = default;
    };

    explicit OrderBook(Ticker ticker) noexcept;

    ~OrderBook() noexcept;

    Ticker getTicker() const noexcept;

    void add(const Order& order) noexcept;

    void add(OrderId id, Side side, Price price, Quantity quantity) noexcept;

    void cancel(OrderId id) noexcept;

    std::optional<Front> getBestBid() const noexcept;

    std::optional<Front> getBestAsk() const noexcept;

    void takeBestBid(Quantity qty) noexcept;

    void takeBestAsk(Quantity qty) noexcept;

private:
    std::optional<Front> front(const auto& map) const noexcept;

    void take(auto& map, Quantity qty) noexcept;

    static void removeOrder(auto& map, RestingOrder& order) noexcept;

    Ticker ticker_{};

    std::map<Price, PriceLevel> asks_;
    std::map<Price, PriceLevel, std::greater<>> bids_;

    std::unordered_map<OrderId, std::unique_ptr<RestingOrder>> orders_;
};
