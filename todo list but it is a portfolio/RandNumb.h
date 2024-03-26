#pragma once
#ifndef RANDNUMB_H
#define RANDNUMB_H

#include <random>
#include <chrono>
#include <limits>

using namespace std;

class RandomNumberGenerator {
private:
    mt19937_64 m_generator;

public:
    RandomNumberGenerator() {
        // Используем случайное зерно, основанное на текущем времени
        unsigned seed = chrono::system_clock::now().time_since_epoch().count();
        m_generator.seed(seed);
    }

    long long getRandomInRange(long long lower, long long upper) {
        uniform_int_distribution<long long> distribution(lower, upper);
        return distribution(m_generator);
    }
};

int RandomNumber();

#endif