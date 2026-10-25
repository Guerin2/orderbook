#include <catch2/catch_test_macros.hpp>
#include "orderbook.hpp"

using S = Side;

// ---- Step 1: resting orders ----------------------------------------------
TEST_CASE("empty book has no best prices") {
    OrderBook b;
    REQUIRE_FALSE(b.bestBid());
    REQUIRE_FALSE(b.bestAsk());
}

TEST_CASE("non-crossing orders rest on the book") {
    OrderBook b;
    REQUIRE(b.addLimit(1, S::Buy, 99, 10).empty());
    REQUIRE(b.addLimit(2, S::Buy, 100, 5).empty());
    REQUIRE(b.addLimit(3, S::Sell, 102, 7).empty());
    REQUIRE(b.bestBid() == 100);
    REQUIRE(b.bestAsk() == 102);
    REQUIRE(b.volumeAt(S::Buy, 99) == 10);
}

// ---- Step 2: matching -----------------------------------------------------
TEST_CASE("crossing order trades at the resting price") {
    OrderBook b;
    b.addLimit(1, S::Sell, 100, 10);
    auto t = b.addLimit(2, S::Buy, 105, 4);
    REQUIRE(t.size() == 1);
    REQUIRE(t[0].price == 100);
    REQUIRE(t[0].qty == 4);
    REQUIRE(b.volumeAt(S::Sell, 100) == 6);   // partial fill of the resting order
    REQUIRE_FALSE(b.bestBid());               // aggressor fully filled, nothing rests
}

TEST_CASE("time priority: earlier order at the same price fills first") {
    OrderBook b;
    b.addLimit(1, S::Sell, 100, 5);
    b.addLimit(2, S::Sell, 100, 5);
    auto t = b.addLimit(3, S::Buy, 100, 7);
    REQUIRE(t.size() == 2);
    REQUIRE(t[0].resting == 1); REQUIRE(t[0].qty == 5);
    REQUIRE(t[1].resting == 2); REQUIRE(t[1].qty == 2);
}

TEST_CASE("sweeps several price levels, then rests the remainder") {
    OrderBook b;
    b.addLimit(1, S::Sell, 100, 3);
    b.addLimit(2, S::Sell, 101, 3);
    b.addLimit(3, S::Sell, 103, 3);
    auto t = b.addLimit(4, S::Buy, 101, 10);
    REQUIRE(t.size() == 2);
    REQUIRE(b.bestAsk() == 103);              // 103 was above the limit
    REQUIRE(b.bestBid() == 101);
    REQUIRE(b.volumeAt(S::Buy, 101) == 4);    // 10 - 3 - 3 rests
}

// ---- Step 3: market orders ------------------------------------------------
TEST_CASE("market order never rests") {
    OrderBook b;
    b.addLimit(1, S::Sell, 100, 3);
    auto t = b.addMarket(2, S::Buy, 10);
    REQUIRE(t.size() == 1);
    REQUIRE(t[0].qty == 3);
    REQUIRE_FALSE(b.bestAsk());
    REQUIRE_FALSE(b.bestBid());
}

// ---- Step 4: cancels ------------------------------------------------------
TEST_CASE("cancel removes a resting order") {
    OrderBook b;
    b.addLimit(1, S::Buy, 100, 5);
    b.addLimit(2, S::Buy, 100, 5);
    REQUIRE(b.cancel(1));
    REQUIRE(b.volumeAt(S::Buy, 100) == 5);
    REQUIRE_FALSE(b.cancel(1));               // can't cancel twice
    REQUIRE_FALSE(b.cancel(999));             // unknown id
}

TEST_CASE("cannot cancel an order that has fully filled") {
    OrderBook b;
    b.addLimit(1, S::Sell, 100, 5);
    b.addLimit(2, S::Buy, 100, 5);
    REQUIRE_FALSE(b.cancel(1));
    REQUIRE_FALSE(b.bestAsk());
}

// TODO: add your own: cancelling the last order at a level removes the level;
// sell-side mirror of the sweep test; zero-quantity orders.
