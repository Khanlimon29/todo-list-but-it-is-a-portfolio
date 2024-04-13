#include <iostream>
#include <chrono>
#include <ctime>
#include <ratio>
#include <cmath>
#include <conio.h>
#include <iomanip>

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
            _getch();
            return;
        }
    }

    int h2, m2, s2;
    cout << "Введите второе время (часы минуты секунды): ";
    cin >> h2 >> m2 >> s2;

    if (!IsValidTime(h2, m2, s2)) {
        cout << "Некорректное время.\n";
        _getch();
        return;
    }

    int totalSeconds1 = h1 * 3600 + m1 * 60 + s1;
    int totalSeconds2 = h2 * 3600 + m2 * 60 + s2;

    int difference = abs(totalSeconds1 - totalSeconds2);

    int hours = difference / 3600;
    int minutes = (difference % 3600) / 60;
    int seconds = difference % 60;

    cout << "Разница между временами: " << setfill('0') << setw(2) << h1 << ":" << setw(2) << m1 << ":" << setw(2) << s1 << " и ";
    cout << setfill('0') << setw(2) << h2 << ":" << setw(2) << m2 << ":" << setw(2) << s2 << " равна ";
    cout << setfill('0') << setw(2) << hours << ":" << setw(2) << minutes << ":" << setw(2) << seconds << endl;
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
        cout << "Введите первую дату (день месяц день): ";
        cin >> day1 >> month1 >> year1;
    }
    if (!IsValidDate(year1, month1, day1)) {
        cout << "Некорректная дата.\n";
        _getch();
        return;
    }

    int year2, month2, day2;
    cout << "Введите вторую дату (день месяц день): ";
    cin >> day2 >> month2 >> year2;

    if (!IsValidDate(year2, month2, day2)) {
        cout << "Некорректная дата.\n";
        _getch();
        return;
    }

    tm time1 = { 0, 0, 0, day1, month1 - 1, year1 - 1900 };
    tm time2 = { 0, 0, 0, day2, month2 - 1, year2 - 1900 };

    time_t time1_tt = mktime(&time1);
    time_t time2_tt = mktime(&time2);

    int difference = difftime(time2_tt, time1_tt) / (24 * 3600);
    int years = difference / 365;
    int months = (difference % 365) / 30;
    int days = (difference % 365) % 30;

    cout << "Разница между датами: " << setfill('0') << setw(2) << day1 << "." << setw(2) << month1 << "." << setw(2) << year1 << " и ";
    cout << setfill('0') << setw(2) << day2 << "." << setw(2) << month2 << "." << setw(2) << year2 << ": ";

    if (years > 0) {
        cout << years << " ";
        if (years == 1 || (years > 20 && years % 10 == 1)) {
            cout << "год, ";
        }
        else if ((years >= 2 && years <= 4) || (years > 20 && (years % 10 >= 2 && years % 10 <= 4))) {
            cout << "года, ";
        }
        else {
            cout << "лет, ";
        }
    }
    if (months > 0) {
        cout << months << " ";
        if (months == 1 || (months > 20 && months % 10 == 1)) {
            cout << "месяц, ";
        }
        else if ((months >= 2 && months <= 4) || (months > 20 && (months % 10 >= 2 && months % 10 <= 4))) {
            cout << "месяца, ";
        }
        else {
            cout << "месяцев, ";
        }
    }
    if (days > 0) {
        cout << days << " ";
        if (days == 1 || (days > 20 && days % 10 == 1)) {
            cout << "день ";
        }
        else if ((days >= 2 && days <= 4) || (days > 20 && (days % 10 >= 2 && days % 10 <= 4))) {
            cout << "дня ";
        }
        else {
            cout << "дней ";
        }
    }
    cout << "\nНажмите на любую кнопку для продолжения";
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
        cout << "Введите первую дату и время (день месяц год часы минуты секунды): ";
        cin >> day1 >> month1 >> year1 >> hour1 >> minute1 >> second1;

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
    cout << "Введите вторую дату и время (день месяц год часы минуты секунды): ";
    cin >> day2 >> month2 >> year2 >> hour2>> minute2 >> second2;

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

    int difference = difftime(time2_tt, time1_tt);

    int days = (difference / (24 * 3600));
    int remainingSeconds = (difference) % (24 * 3600);
    int years = days / 365;
    days %= 365;
    int hours = remainingSeconds / 3600;
    remainingSeconds %= 3600;
    int minutes = remainingSeconds / 60;
    int seconds = remainingSeconds % 60;

    cout << "Разница между днями и временем: " << setfill('0') << setw(2) << day1 << "." << setw(2) << month1 << "." << setw(2) << year1 << setw(2) << " " << hour1 << ":" << setw(2) << minute1 << ":" << setw(2) << second1 << " и ";
        cout << setfill('0') << setw(2) << day2 << "." << setw(2) << month2 << "." << setw(2) << year2 << " " << setw(2) << hour2 << ":" << setw(2) << minute2 << ":" << setw(2) << second2 << ": ";

    if (years > 0) {
        cout << years << " ";
        if (years == 1 || (years > 20 && years % 10 == 1)) {
            cout << "год, ";
        }
        else if ((years >= 2 && years <= 4) || (years > 20 && (years % 10 >= 2 && years % 10 <= 4))) {
            cout << "года, ";
        }
        else {
            cout << "лет, ";
        }
    }
    cout << days << " ";
    if (days == 1 || (days > 20 && days % 10 == 1)) {
        cout << "день, ";
    }
    else if ((days >= 2 && days <= 4) || (days > 20 && (days % 10 >= 2 && days % 10 <= 4))) {
        cout << "дня, ";
    }
    else {
        cout << "дней, ";
    }
    cout << hours << " ";
    if (hours == 1 || hours == 21) {
        cout << "час, ";
    }
    else if ((hours >= 2 && hours <= 4) || (hours >= 22 && hours <= 23) || (hours >= 0 && hours <= 4)) {
        cout << "часа, ";
    }
    else {
        cout << "часов, ";
    }
    cout << minutes << " ";
    if (minutes == 1 || (minutes > 20 && minutes % 10 == 1)) {
        cout << "минута, ";
    }
    else if ((minutes >= 2 && minutes <= 4) || (minutes > 20 && (minutes % 10 >= 2 && minutes % 10 <= 4))) {
        cout << "минуты, ";
    }
    else {
        cout << "минут, ";
    }
    cout << seconds << " ";
    if (seconds == 1 || (seconds > 20 && seconds % 10 == 1)) {
        cout << "секунда ";
    }
    else if ((seconds >= 2 && seconds <= 4) || (seconds > 20 && (seconds % 10 >= 2 && seconds % 10 <= 4))) {
        cout << "секунды ";
    }
    else {
        cout << "секунд ";
    }
    cout << "\nНажмите на любую кнопку для продолжения";
    _getch();
}

void DateMenu(bool CurrentTime, int currentYear, int currentMonth, int currentDay, int currentHour, int currentMinute, int currentSecond) {
    cout << "Выбранный вариант: Программа для работы с датами и временем: Программа для выполнения операций с датами и временем, таких как расчет разницы между датами.\n\n";
    cout << "Режим работы (24H only): ";
    if (CurrentTime == true) cout << "От текущего времени";
    else cout << "От заданного времени";
    cout << "\nТекущая дата и время (День, месяц, год, часы, минуты, секунды): ";
    cout << setfill('0') << setw(2) << currentDay << "." << setw(2) << currentMonth << "." << currentYear << " "
         << setw(2) << currentHour << ":" << setw(2) << currentMinute << ":" << setw(2) << currentSecond << "\n\n";
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