#include "orderbook.hpp"

// ---------------------------------------------------------------------------
// Your job: implement these. Suggested order:
//   1. addLimit with NO matching (just rest the order) + bestBid/bestAsk/volumeAt
//   2. matching in addLimit: while the order crosses the best opposite level,
//      fill against the FRONT of that level's queue (time priority), pop filled
//      orders, erase empty levels
//   3. addMarket (same matching loop, no price limit, never rests)
//   4. cancel
// Run the tests after each step; they're ordered the same way.
// ---------------------------------------------------------------------------

std::vector<Trade> OrderBook::addLimit(OrderId, Side, Price, Quantity) {
    return {};  // TODO
}

std::vector<Trade> OrderBook::addMarket(OrderId, Side, Quantity) {
    return {};  // TODO
}

bool OrderBook::cancel(OrderId) {
    return false;  // TODO
}

std::optional<Price> OrderBook::bestBid() const {
    return std::nullopt;  // TODO
}

std::optional<Price> OrderBook::bestAsk() const {
    return std::nullopt;  // TODO
}

Quantity OrderBook::volumeAt(Side, Price) const {
    return 0;  // TODO
}
