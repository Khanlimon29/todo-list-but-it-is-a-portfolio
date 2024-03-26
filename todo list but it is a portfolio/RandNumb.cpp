#include "RandNumb.h"
#include <iostream>
#include <conio.h>

using namespace std;

int RandomNumber() {
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

    return 0;
}
