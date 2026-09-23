#pragma once

#include <vector>
#include <random>
#include <concepts>
#include <type_traits>

class randfiller {
private:
    std::mt19937 gen;

public:
    randfiller()
        : gen(std::random_device{}())
    {}

    explicit randfiller(unsigned int seed)
        : gen(seed)
    {}

    void reseed() {
        gen.seed(std::random_device{}());
    }

    void reseed(unsigned int seed) {
        gen.seed(seed);
    }

    template <typename T> requires std::integral<T>
    void fill(std::vector<T>& vec, T min, T max) {
        std::uniform_int_distribution<T> dist(min, max);
        for (auto& v : vec) {
            v = dist(gen);
        }
    }

    template <typename T> requires std::floating_point<T>
    void fill(std::vector<T>& vec, T min, T max) {
        std::uniform_real_distribution<T> dist(min, max);
        for (auto& v : vec) {
            v = dist(gen);
        }
    }
};