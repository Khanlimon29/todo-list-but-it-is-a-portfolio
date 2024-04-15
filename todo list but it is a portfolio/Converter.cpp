#include <iostream>
#include <conio.h>
#include <map>
#include <iomanip>
#include <vector>
#include "gotoxy.h"

using namespace std;

map<string, map<string, double>> ConversionTable = {
    {"Distance", {
        {"Миллиметр", 1000},        // Миллиметр = 1/1000 метра
        {"Сантиметр", 100},         // Сантиметр = 1/100 метра
        {"Метр", 1},                // Метр = 1 метра
        {"Километр", 0.001},        // Километр = 1000 метров
        {"Дюйм", 39.3701},          // Дюйм = 1/39.3701 метра
        {"Фут", 3.28084},           // Фут = 1/3.28084 метра
        {"Ярд", 1.09361},           // Ярд = 1/1.09361 метра
        {"Миля", 0.000621371},      // Миля = 1609.34 метра
        {"Морская миля", 0.000539957} // Морская миля = 1852 метра
    }},
    {"Mass", {
        {"Грамм", 1000},            // Грамм = 1/1000 килограмма
        {"Килограмм", 1},           // Килограмм = 1 килограмма
        {"Тонна", 0.001},           // Тонна = 1000 килограмм
        {"Унция", 35.274},          // Унция = 1/35.274 килограмма
        {"Фунт", 2.20462}           // Фунт = 1/2.20462 килограмма
    }},
    {"Speed", {
        {"Число Маха", 295},              // Число Маха = 295 метра в секунду (СИ) (высота 11км)
        {"Метр в секунду", 1},            // Метр в секунду = 1 метр в секунду
        {"Метр в минуту", 0.01666},       // Метр в минуту = 60 метров в секунду
        {"Километр в секунду", 1000},     // Километр в секунду = 1/1000 метра в секунду
        {"Километр в час", 0.277778},     // Километр в час = 0.277778 метра в секунду                        // было плохой идеей доверять такую работу чатгпт...
        {"Миля в секунду", 1610.3059},    // Миля в секунду = 1610.3059 метра в секунду
        {"Миля в час", 0.44704},          // Миля в час = 0.44704 метра в секунду
        {"Фут в секунду", 0.3048},        // Фут в секунду = 0.3048 метра в секунду
        {"Фут в час", 0.000846},          // Фут в час = 0.000846 метра в секунду
        {"Узел", 0.514444},               // Узел = 0.514444 метра в секунду    
    }}
};

void ConverterMenu() {
    cout << "Выбранный вариант: Конвертер единиц измерения: Программа, конвертирующая значения между различными единицами измерения (например, длина, вес, объем).\n\n";
    cout << "Нажмите на нужную цифру для управления:\n";
    cout << "1. Конвертер расстояния\n2. Конвертер массы\n3. Конвертер скорости\n\nEsc. Выход";
}

void ConvertDistance() {
    cout << "Конвертер расстояния:\n";
    cout << "Выберите начальную единицу измерения:\n";
    vector<string> distanceUnits = { "Миллиметр", "Сантиметр", "Метр", "Километр", "Дюйм", "Фут", "Ярд", "Миля", "Морская миля" };
    for (int i = 0; i < distanceUnits.size(); ++i) {
        cout << i + 1 << ". " << distanceUnits[i] << endl;
    }
    cout << "Esc. Выход";
    char unitChoice = _getch();
    if (unitChoice >= '1' && unitChoice <= '9') {
        int unitIndex = unitChoice - '0';
        if (unitIndex >= 1 && unitIndex <= 9) {
            string fromUnit = distanceUnits[unitIndex - 1];
            system("cls");
            cout << "Введите значение в " << fromUnit << ": ";
            double value;
            cin >> value;
            double valueInMeters = value / ConversionTable["Distance"][fromUnit];
            cout << "\nТаблица конвертации:\n";
            cout << "Единица измерения   |   Значение\n";
            cout << "----------------------------------\n";
            int i = 5;
            for (auto& unit : distanceUnits) {
                cout << unit;
                gotoxy(17, i);   // костыльно, но что поделать?
                cout << "   |   " << fixed << setprecision(6) << valueInMeters * ConversionTable["Distance"][unit] << endl;
                i++;
            }
            cout << "\nНажмите любую клавишу для продолжения";
            _getch();
            system("cls");
            ConverterMenu();
        }
    }
    else if (unitChoice == 27) {
        system("cls");
        ConverterMenu();
    }
}

void ConvertMass() {
    cout << "Конвертер массы:\n";
    cout << "Выберите начальную единицу измерения:\n";
    vector<string> massUnits = { "Грамм", "Килограмм", "Тонна", "Унция", "Фунт" };
    for (int i = 0; i < massUnits.size(); ++i) {
        cout << i + 1 << ". " << massUnits[i] << endl;
    }
    cout << "Esc. Выход";
    char unitChoice = _getch();
    if (unitChoice >= '1' && unitChoice <= '5') {
        int unitIndex = unitChoice - '0';
        if (unitIndex >= 1 && unitIndex <= 5) {
            string fromUnit = massUnits[unitIndex - 1];
            system("cls");
            cout << "Введите значение в " << fromUnit << ": ";
            double value;
            cin >> value;
            double valueInKilograms = value / ConversionTable["Mass"][fromUnit];
            cout << "\nТаблица конвертации:\n";
            cout << "Единица измерения   |   Значение\n";
            cout << "----------------------------------\n";
            int i = 5;
            for (auto& unit : massUnits) {
                cout << unit;
                gotoxy(17, i);   // костыльно, но что поделать?
                cout << "   |   " << fixed << setprecision(6) << valueInKilograms * ConversionTable["Mass"][unit] << endl;
                i++;
            }
            cout << "\nНажмите любую клавишу для продолжения";
            _getch();
            system("cls");
            ConverterMenu();
        }
    }
    else if (unitChoice == 27) {
        system("cls");
        ConverterMenu();
    }
}

void ConvertSpeed() {
    cout << "Конвертер скорости:\n";
    cout << "Выберите начальную единицу измерения:\n";
    vector<string> speedUnits = { "Число Маха", "Метр в секунду", "Метр в минуту", "Километр в секунду", "Километр в час", "Миля в секунду", "Миля в час", "Фут в секунду", "Фут в час", "Узел" };
    for (int i = 0; i < speedUnits.size(); ++i) {
        cout << i << ". " << speedUnits[i] << endl;
    }
    cout << "Esc. Выход";
    char unitChoice = _getch();
    if (unitChoice >= '0' && unitChoice <= '9') {
        int unitIndex = unitChoice - '0';
        if (unitIndex >= 0 && unitIndex <= 9) {
            string fromUnit = speedUnits[unitIndex];
            system("cls");
            cout << "Введите значение в " << fromUnit << ": ";
            double value;
            cin >> value;
            cout << "\nТаблица конвертации:\n";
            cout << "Единица измерения   |   Значение\n";
            cout << "----------------------------------\n";
            int i = 5;
            for (auto& unit : speedUnits) {
                cout << unit;
                gotoxy(17, i);   // костыльно, но что поделать?
                cout << "   |   " << fixed << setprecision(6) << value * ConversionTable["Speed"][fromUnit] / ConversionTable["Speed"][unit] << endl;
                i++;
            }
            cout << "\nНажмите любую клавишу для продолжения";
            _getch();
            system("cls");
            ConverterMenu();
        }
    }
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
            ConvertDistance();
            break;
        }
        case '2': {
            system("cls");
            ConvertMass();
            break;
        }
        case '3': {
            system("cls");
            ConvertSpeed();
            break;
        }
        case 27: {
            running = false;
            break;
        }
        }
    }
}