//
// Created by Will George on 3/22/26.
//

#include <orderbook/core/matching_engine.hpp>

MatchingEngine::MatchingEngine(Ticker ticker) noexcept : ticker_(ticker), order_book_(ticker) {}

Ticker MatchingEngine::getTicker() const noexcept {
    return ticker_;
}
