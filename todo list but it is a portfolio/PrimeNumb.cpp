#include "PrimeNumb.h"
#include <iostream>
#include <conio.h>
#include <vector>
using namespace std;

vector<int> FindPrimes(int lower, int upper) {
    vector<bool> isPrime(upper + 1, true);
    vector<int> primes;

    for (int i = lower; i * i <= upper; ++i) {
        if (isPrime[i]) {
            for (int j = i * i; j <= upper; j += i) {
                isPrime[j] = false;
            }
        }
    }

    for (int i = lower; i <= upper; ++i) {
        if (isPrime[i]) {
            primes.push_back(i);
        }
    }

    return primes;
}

void Finder() {
    int lower, upper;
    cout << "Введите диапазон для нахождения простых чисел (нижний порог должен быть больше 2): ";
    cin >> lower >> upper;

    while (cin.fail() || lower >= upper || lower < 2 || upper < 0) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Введите значения повторно: ";
        cin >> lower >> upper;
    }

    vector<int> primes = FindPrimes(lower, upper);

    cout << "Простые числа в диапазоне [" << lower << ", " << upper << "]:" << endl;
    for (int prime : primes) {
        cout << prime << " ";
    }
    cout << endl;

    _getch();
}