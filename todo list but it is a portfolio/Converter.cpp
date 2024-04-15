#include <iostream>
#include <conio.h>
#include <map>
#include <iomanip>

using namespace std;

void ConverterMenu() {
    cout << "Выбранный вариант: Конвертер единиц измерения: Программа, конвертирующая значения между различными единицами измерения (например, длина, вес, объем).\n\n";
    cout << "1. Конвертер расстояния\n2. Конвертер массы\n3. Конвертер скорости\n\n";
    cout << "Выберите раздел (или нажмите Esc для выхода): ";
}

void Converter() {
    system("cls");
    ConverterMenu();
    char choice;
    bool running = true;
    while (running) {
        choice = _getch();
        switch (choice) {
        case '1': {
            system("cls");

            _getch();
            system("cls");
            ConverterMenu();
            break;
        }
        case '2': {
            system("cls");

            _getch();
            system("cls");
            ConverterMenu();
            break;
        }
        case '3': {
            system("cls");

            _getch();
            system("cls");
            ConverterMenu();
            break;
        }
        case 27: {
            running = false;
            break;
        }
        }
    }
}