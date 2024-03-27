#include "PasswordGen.h"
#include "RandNumb.h"
#include <iostream>
#include <conio.h>
using namespace std;

void Generator() {
	string alphabet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()_+-=[]{}|;:,.<>?";
	
    long long lenght;
    RandomNumberGenerator rng;

    cout << "Введите длину пароля: ";
    cin >> lenght;

    while (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Введите значения повторно: ";
        cin >> lenght;
    }

    cout << "Ваш пароль: ";
    for (int i = 0; i < lenght; i++) cout << alphabet[rng.getRandomInRange(1, alphabet.size())];
    cout << "\nНажмите на любую кнопку для продолжения";
	_getch();
}
