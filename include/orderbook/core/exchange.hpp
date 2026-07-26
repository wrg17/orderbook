//
// Created by Will George on 7/21/26.
//
#ifndef ORDERBOOK_CORE_EXCHANGE_HPP
#define ORDERBOOK_CORE_EXCHANGE_HPP

#include <orderbook/core/matching_engine.hpp>
#include <orderbook/domain/order.hpp>
#include <orderbook/domain/trade.hpp>

#include <map>
#include <optional>
#include <vector>

class Exchange {
public:
    Exchange() noexcept;
    ~Exchange() noexcept = default;

    Exchange(const Exchange& other) = delete;
    Exchange(Exchange&& other) = delete;

    Exchange& operator=(const Exchange& other) = delete;
    Exchange& operator=(Exchange&& other) = delete;

    bool list(Ticker ticker) noexcept;

    [[nodiscard]] bool isListed(Ticker ticker) const noexcept;

    [[nodiscard]] std::optional<std::vector<Trade>> submit(const Order& order) noexcept;

    [[nodiscard]] std::optional<bool> cancel(Ticker ticker, OrderId id) noexcept;

private:
    std::map<Ticker, MatchingEngine> engines_;
};

#endif // ORDERBOOK_CORE_EXCHANGE_HPP
