//
// Created by Will George on 3/22/26.
//

#include <cstdlib>
#include <format>
#include <orderbook/domain/trade.hpp>

Trade::Trade(OrderId maker, OrderId taker, Side side, Ticker ticker, Price price, Quantity quantity,
             std::chrono::system_clock::time_point time) noexcept
    : id_(TradeId{++id_seq}), maker_order_id_(maker), taker_order_id_(taker), taker_side_(side),
      ticker_(ticker), price_(price), quantity_(quantity), time_(time) {}

TradeId Trade::getId() const noexcept {
    return id_;
}

OrderId Trade::getMakerOrderId() const noexcept {
    return maker_order_id_;
}

OrderId Trade::getTakerOrderId() const noexcept {
    return taker_order_id_;
}

Side Trade::getTakerSide() const noexcept {
    return taker_side_;
}

Ticker Trade::getTicker() const noexcept {
    return ticker_;
}

Price Trade::getPrice() const noexcept {
    return price_;
}

Quantity Trade::getQuantity() const noexcept {
    return quantity_;
}

std::chrono::system_clock::time_point Trade::getTime() const noexcept {
    return time_;
}

std::ostream& operator<<(std::ostream& os, const Trade& trade) noexcept {
    try {
        return os << std::format("{}", trade);
    } catch (...) {   // GCOVR_EXCL_LINE
        std::abort(); // GCOVR_EXCL_LINE
    }
}

std::format_context::iterator
std::formatter<TradeId>::format(TradeId id, std::format_context& ctx) const noexcept {
    try {
        return std::formatter<std::uint64_t>::format(id.value, ctx);
    } catch (...) {   // GCOVR_EXCL_LINE
        std::abort(); // GCOVR_EXCL_LINE
    }
}

std::format_context::iterator std::formatter<Trade>::format(const Trade& trade,
                                                            std::format_context& ctx) noexcept {
    try {
        return std::format_to(
            ctx.out(),
            "Trade(Id:{},Maker:{},Taker:{},Side:{},Ticker:{},Price:{},Quantity:{},Time:{})",
            trade.getId(), trade.getMakerOrderId(), trade.getTakerOrderId(), trade.getTakerSide(),
            trade.getTicker(), trade.getPrice(), trade.getQuantity(),
            trade.getTime().time_since_epoch().count());
    } catch (...) {   // GCOVR_EXCL_LINE
        std::abort(); // GCOVR_EXCL_LINE
    }
}
