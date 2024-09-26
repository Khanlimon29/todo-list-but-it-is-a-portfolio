#pragma once
#ifndef RANDNUMB_H
#define RANDNUMB_H

#include <random>

using namespace std;

class RandomNumberGenerator {
private:
    mt19937_64 m_generator;

public:
    RandomNumberGenerator();
    long long getRandomInRange(long long lower, long long upper);
};

void RandomNumber();

#endif