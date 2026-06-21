//
// Created by Will George on 3/22/26.
//
#pragma once

#include <orderbook/core/order_book.hpp>

class MatchingEngine {
public:
    explicit MatchingEngine(Ticker ticker) noexcept;
    ~MatchingEngine() noexcept = default;

    MatchingEngine(const MatchingEngine& other) = delete;
    MatchingEngine(MatchingEngine&& other) = delete;

    MatchingEngine& operator=(const MatchingEngine& other) = delete;
    MatchingEngine& operator=(MatchingEngine&& other) = delete;

    [[nodiscard]] Ticker getTicker() const noexcept;

private:
    Ticker ticker_;
    OrderBook order_book_;
};
