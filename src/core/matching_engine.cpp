//
// Created by Will George on 3/22/26.
//

#include <orderbook/core/matching_engine.hpp>

#include <algorithm>
#include <chrono>

MatchingEngine::MatchingEngine(Ticker ticker) noexcept : ticker_(ticker) {}

Ticker MatchingEngine::getTicker() const noexcept {
    return ticker_;
}

std::vector<Trade> MatchingEngine::submit(const Order& order) noexcept {
    std::vector<Trade> trades;

    const auto kGet =
        order.getSide() == Side::BUY ? &OrderBook::getBestAsk : &OrderBook::getBestBid;
    const auto kTake =
        order.getSide() == Side::BUY ? &OrderBook::takeBestAsk : &OrderBook::takeBestBid;

    Quantity need = order.getQuantity();

    while (need > Quantity{0}) {
        const std::optional<OrderBook::Front> kBest = (order_book_.*kGet)();
        if (!kBest.has_value() || !isMatch(order, *kBest)) {
            break;
        }

        const Quantity kFill = std::min(need, kBest->quantity);
        (order_book_.*kTake)(kFill);

        trades.emplace_back(kBest->id, order.getId(), order.getSide(), getTicker(), kBest->price,
                            kFill, std::chrono::system_clock::now());
        need -= kFill;
    }

    if (need > Quantity{0}) {
        order_book_.add(order.getId(), order.getSide(), order.getPrice(), need);
    }

    return trades;
}

bool MatchingEngine::cancel(OrderId id) noexcept {
    return order_book_.cancel(id);
}

bool MatchingEngine::isMatch(const Order& aggressor, const OrderBook::Front& resting) noexcept {
    if (aggressor.getSide() == Side::BUY) {
        return resting.price <= aggressor.getPrice();
    }
    return resting.price >= aggressor.getPrice();
}
