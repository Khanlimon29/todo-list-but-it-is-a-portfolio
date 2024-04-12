#include <iostream>
#include <chrono>
#include <ctime>
#include <ratio>
#include <cmath>
#include <conio.h>

using namespace std;
using namespace chrono;

bool IsValidTime(int h, int m, int s) {
    return (h >= 0 && h < 24) && (m >= 0 && m < 60) && (s >= 0 && s < 60);
}

bool IsValidDate(int year, int month, int day) {
    if (month < 1 || month > 12)
        return false;

    int daysInMonth[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    int maxDay = daysInMonth[month - 1];

    if (month == 2 && (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)))
        maxDay = 29; // Високосный февраль

    return day >= 1 && day <= maxDay;
}

void CalculateTimeDifferenceWithinDay(bool CurrentTime, int currentHour, int currentMinute, int currentSecond) {
    int h1, m1, s1;

    if (CurrentTime == true) {
        h1 = currentHour;
        m1 = currentMinute;
        s1 = currentSecond;
    }
    else {
        cout << "Введите первое время (часы минуты секунды): ";
        cin >> h1 >> m1 >> s1;

        if (!IsValidTime(h1, m1, s1)) {
            cout << "Некорректное время.\n";
            return;
        }
    }

    int h2, m2, s2;
    cout << "Введите второе время (часы минуты секунды): ";
    cin >> h2 >> m2 >> s2;

    if (!IsValidTime(h2, m2, s2)) {
        cout << "Некорректное время.\n";
        return;
    }

    int totalSeconds1 = h1 * 3600 + m1 * 60 + s1;
    int totalSeconds2 = h2 * 3600 + m2 * 60 + s2;

    int difference = abs(totalSeconds1 - totalSeconds2);

    int hours = difference / 3600;
    int minutes = (difference % 3600) / 60;
    int seconds = difference % 60;

    cout << "Разница между временами: " << hours << " часов " << minutes << " минут " << seconds << " секунд\n";
    cout << "Нажмите на любую кнопку для продолжения";
    _getch();
}

void CalculateDateDifferenceInDays(bool CurrentTime, int currentYear, int currentMonth, int currentDay) {
    int year1, month1, day1;

    if (CurrentTime == true) {
        year1 = currentYear;
        month1 = currentMonth;
        day1 = currentDay;
    }
    else {
        cout << "Введите первую дату (год месяц день): ";
        cin >> year1 >> month1 >> day1;
    }
    if (!IsValidDate(year1, month1, day1)) {
        cout << "Некорректная дата.\n";
        return;
    }

    int year2, month2, day2;
    cout << "Введите вторую дату (год месяц день): ";
    cin >> year2 >> month2 >> day2;

    if (!IsValidDate(year2, month2, day2)) {
        cout << "Некорректная дата.\n";
        _getch();
        return;
    }

    tm time1 = { 0, 0, 0, day1, month1 - 1, year1 - 1900 };
    tm time2 = { 0, 0, 0, day2, month2 - 1, year2 - 1900 };

    time_t time1_tt = mktime(&time1);
    time_t time2_tt = mktime(&time2);

    int difference = abs(int(difftime(time2_tt, time1_tt) / (24 * 3600)));

    cout << "Разница между датами: " << difference << " дней\n";
    cout << "Нажмите на любую кнопку для продолжения";
    _getch();
}

void CalculateDayDifferenceInSeconds(bool CurrentTime, int currentYear, int currentMonth, int currentDay, int currentHour, int currentMinute, int currentSecond) {
    int year1, month1, day1, hour1, minute1, second1;

    if (CurrentTime == true) {
        year1 = currentYear;
        month1 = currentMonth;
        day1 = currentDay;
        hour1 = currentHour;
        minute1 = currentMinute;
        second1 = currentSecond;
    }
    else {
        cout << "Введите первую дату и время (год месяц день часы минуты секунды): ";
        cin >> year1 >> month1 >> day1 >> hour1 >> minute1 >> second1;

        if (!IsValidDate(year1, month1, day1)) {
            cout << "Некорректная дата.\n";
            _getch();
            return;
        }

        if (!IsValidTime(hour1, minute1, second1)) {
            cout << "Некорректное время.\n";
            _getch();
            return;
        }
    }

    int year2, month2, day2, hour2, minute2, second2;
    cout << "Введите вторую дату и время (год месяц день часы минуты секунды): ";
    cin >> year2 >> month2 >> day2 >> hour2 >> minute2 >> second2;

    if (!IsValidDate(year2, month2, day2)) {
        cout << "Некорректная дата.\n";
        _getch();
        return;
    }

    if (!IsValidTime(hour2, minute2, second2)) {
        cout << "Некорректное время.\n";
        _getch();
        return;
    }

    tm time1 = { second1, minute1, hour1, day1, month1 - 1, year1 - 1900 };
    tm time2 = { second2, minute2, hour2, day2, month2 - 1, year2 - 1900 };

    time_t time1_tt = mktime(&time1);
    time_t time2_tt = mktime(&time2);

    int difference = abs(int(difftime(time2_tt, time1_tt)));

    int days = difference / (24 * 3600);
    int remainingSeconds = difference % (24 * 3600);
    int hours = remainingSeconds / 3600;
    remainingSeconds %= 3600;
    int minutes = remainingSeconds / 60;
    int seconds = remainingSeconds % 60;

    cout << "Разница между днями: " << days << " дней, " << hours << " часов, " << minutes << " минут, " << seconds << " секунд\n";
    cout << "Нажмите на любую кнопку для продолжения";
    _getch();
}

void DateMenu(bool CurrentTime, int currentYear, int currentMonth, int currentDay, int currentHour, int currentMinute, int currentSecond) {
    cout << "Выбранный вариант: Программа для работы с датами и временем: Программа для выполнения операций с датами и временем, таких как расчет разницы между датами.\n\n";
    cout << "Режим работы: ";
    if (CurrentTime == true) cout << "От текущего времени";
    else cout << "От заданного времени";
    cout << "\nТекущая дата и время (Год, месяц, день, часы, минуты, секунды): ";
    cout << currentYear << " " << currentMonth << " " << currentDay << " " << currentHour << " " << currentMinute << " " << currentSecond << "\n\n";
    cout << "1. Рассчитать разницу между временами в пределе одного дня\n2. Рассчитать разницу между датами в днях\n3. Рассчитать разницу между днями с точностью до секунд\n\nEnter. Изменение режима работы\nEsc. Выход";
}

void DateFinder() {
    auto now = system_clock::to_time_t(system_clock::now());
    tm* local_time = localtime(&now);
    int currentYear = local_time->tm_year + 1900;
    int currentMonth = local_time->tm_mon + 1;
    int currentDay = local_time->tm_mday;
    int currentHour = local_time->tm_hour;
    int currentMinute = local_time->tm_min;
    int currentSecond = local_time->tm_sec;
    bool CurrentTime = false;
    char choice;
    bool running = true;
    while (running) {
        system("cls");
        DateMenu(CurrentTime, currentYear, currentMonth, currentDay, currentHour, currentMinute, currentSecond);
        choice = _getch();
        switch (choice) {
        case '1': {
            system("cls");
            CalculateTimeDifferenceWithinDay(CurrentTime, currentHour, currentMinute, currentSecond);
            break;
        }
        case '2': {
            system("cls");
            CalculateDateDifferenceInDays(CurrentTime, currentYear, currentMonth, currentDay);
            break;
        }
        case '3': {
            system("cls");
            CalculateDayDifferenceInSeconds(CurrentTime, currentYear, currentMonth, currentDay, currentHour, currentMinute, currentSecond);
            break;
        }
        case 27: {
            running = false;
            break;
        }
        case 13: {
            system("cls");
            CurrentTime = !CurrentTime;
            break;
        }
        }
    }
}