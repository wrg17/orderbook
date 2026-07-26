//
// Created by Will George on 7/21/26.
//

#include <orderbook/core/exchange.hpp>

Exchange::Exchange() noexcept = default;

bool Exchange::list(Ticker ticker) noexcept {
    return engines_.try_emplace(ticker, ticker).second;
}

bool Exchange::isListed(Ticker ticker) const noexcept {
    return engines_.contains(ticker);
}

std::optional<std::vector<Trade>> Exchange::submit(const Order& order) noexcept {
    const auto kIt = engines_.find(order.getTicker());
    if (kIt == engines_.end()) {
        return std::nullopt;
    }
    return kIt->second.submit(order);
}

std::optional<bool> Exchange::cancel(Ticker ticker, OrderId id) noexcept {
    const auto kIt = engines_.find(ticker);
    if (kIt == engines_.end()) {
        return std::nullopt;
    }
    return kIt->second.cancel(id);
}
