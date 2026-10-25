# Limit Order Book

A price-time priority matching engine in C++20.

## Build and test
    cmake -B build
    cmake --build build
    ./build/tests

## Design
TODO: data structures you chose and why.

## Performance
| Version | Throughput (orders/s) | p50 latency | p99 latency |
|---------|-----------------------|-------------|-------------|
| v1: std::map + deque | | | |
