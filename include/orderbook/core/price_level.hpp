//
// Created by Will George on 6/7/26.
//

#pragma once

#include <orderbook/domain/order.hpp>

class PriceLevel;

class RestingOrder {
public:
    RestingOrder(OrderId id, Quantity quantity, Side side) noexcept;
    ~RestingOrder() noexcept;

    RestingOrder(const RestingOrder& other) = delete;
    RestingOrder(RestingOrder&& other) = delete;

    RestingOrder& operator=(const RestingOrder& other) = delete;
    RestingOrder& operator=(const RestingOrder&& other) = delete;

    [[nodiscard]] OrderId getId() const noexcept;

    [[nodiscard]] Side getSide() const noexcept;

    [[nodiscard]] PriceLevel* getLevel() const noexcept;

    [[nodiscard]] RestingOrder* getNext() const noexcept;
    [[nodiscard]] RestingOrder* getPrev() const noexcept;

    [[nodiscard]] Quantity getQuantity() const noexcept;

    void unlink() noexcept;

    void reduce(Quantity reduction) noexcept;

private:
    friend class PriceLevel;

    OrderId id_{};
    Quantity quantity_{};
    Side side_{};

    RestingOrder* next_ = nullptr;
    RestingOrder* prev_ = nullptr;
    PriceLevel* level_ = nullptr;
};

class PriceLevel {
public:
    explicit PriceLevel(Price price) noexcept;
    ~PriceLevel() noexcept;

    PriceLevel(const PriceLevel& other) = delete;
    PriceLevel(PriceLevel&& other) = delete;

    PriceLevel& operator=(const PriceLevel& other) = delete;
    PriceLevel& operator=(const PriceLevel&& other) = delete;

    [[nodiscard]] bool empty() const noexcept;

    [[nodiscard]] Price getPrice() const noexcept;

    [[nodiscard]] RestingOrder* getFront() const noexcept;

    [[nodiscard]] Quantity getTotalQuantity() const noexcept;

    [[nodiscard]] std::size_t getOrderCount() const noexcept;

    void pushBack(RestingOrder& order) noexcept;

private:
    friend class RestingOrder;

    Price price_;
    std::size_t order_count_;
    Quantity total_quantity_;
    RestingOrder sentinel_;
};
