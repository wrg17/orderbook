//
// Created by Will George on 3/22/26.
//
#pragma once

#include <orderbook/core/order_book.hpp>
#include <orderbook/domain/trade.hpp>

#include <vector>

class MatchingEngine {
public:
    explicit MatchingEngine(Ticker ticker) noexcept;
    ~MatchingEngine() noexcept = default;

    MatchingEngine(const MatchingEngine& other) = delete;
    MatchingEngine(MatchingEngine&& other) = delete;

    MatchingEngine& operator=(const MatchingEngine& other) = delete;
    MatchingEngine& operator=(MatchingEngine&& other) = delete;

    [[nodiscard]] Ticker getTicker() const noexcept;

    [[nodiscard]] std::vector<Trade> submit(const Order& order) noexcept;

private:
    [[nodiscard]] static bool isMatch(const Order& aggressor,
                                      const OrderBook::Front& resting) noexcept;

    Ticker ticker_;
    OrderBook order_book_;
};
