#include "RandNumb.h"
#include <iostream>
#include <conio.h>
#include <chrono>

using namespace std;

RandomNumberGenerator::RandomNumberGenerator()
{
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    m_generator.seed(seed);
}

long long RandomNumberGenerator::getRandomInRange(long long lower, long long upper)
{
    uniform_int_distribution<long long> distribution(lower, upper);
    return distribution(m_generator);
}

void RandomNumber() {
    long long lower, upper;
    RandomNumberGenerator rng;

    cout << "Введите диапазон для рандомного числа (нижняя граница, верхняя граница): ";
    cin >> lower >> upper;

    while (cin.fail() || lower >= upper) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Введите значения повторно: ";
        cin >> lower >> upper;
    }

    cout << "\nВаше число: " << rng.getRandomInRange(lower, upper);
    cout << "\nНажмите на любую кнопку для продолжения";
    _getch();
}
