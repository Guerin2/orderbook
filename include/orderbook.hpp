#pragma once
#include <cstdint>
#include <deque>
#include <map>
#include <optional>
#include <unordered_map>
#include <vector>

using OrderId  = std::uint64_t;
using Price    = std::int64_t;   // in ticks (e.g. pence), never floating point
using Quantity = std::uint32_t;

enum class Side { Buy, Sell };

struct Order {
    OrderId  id;
    Side     side;
    Price    price;
    Quantity qty;     // remaining quantity
};

// One fill: an incoming (aggressive) order traded against a resting order.
struct Trade {
    OrderId  aggressor;
    OrderId  resting;
    Price    price;   // trades happen at the RESTING order's price
    Quantity qty;
};

class OrderBook {
public:
    // Match against the opposite side; any remainder rests on the book.
    std::vector<Trade> addLimit(OrderId id, Side side, Price price, Quantity qty);

    // Match against the opposite side; any remainder is discarded (never rests).
    std::vector<Trade> addMarket(OrderId id, Side side, Quantity qty);

    // Remove a resting order. Returns false if it isn't on the book
    // (unknown id, already fully filled, or already cancelled).
    bool cancel(OrderId id);

    std::optional<Price> bestBid() const;
    std::optional<Price> bestAsk() const;
    Quantity volumeAt(Side side, Price price) const;   // total resting qty at a level

private:
    // Version 1: simple and correct. Optimise later, with benchmarks to prove it.
    // Bids: highest price first. Asks: lowest price first.
    std::map<Price, std::deque<Order>, std::greater<Price>> bids_;
    std::map<Price, std::deque<Order>>                      asks_;

    // TODO (cancel in O(1)): you'll want a lookup from OrderId to where the order
    // lives. Think about why std::deque iterators make this awkward, and whether
    // std::list fits better.
    std::unordered_map<OrderId, Side> index_;
};
