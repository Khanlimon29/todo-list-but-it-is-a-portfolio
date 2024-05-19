#include "WeekOfEducation.h"
#include <iostream>
#include <conio.h>
#include <ctime>
#include <iomanip>
#include "gotoxy.h"
#include "setcolor.h"

using namespace std;

int WeekCalculation() {
    time_t now = time(0);
    tm* ltm = localtime(&now);
    tm start_of_semester;
    tm end_of_semester;

    time_t start_semester_time;
    time_t end_semester_time;

    if (ltm->tm_mon < 8) {
        start_of_semester = { 0, 0, 0, 9, 1, ltm->tm_year }; // 9 февраля
        end_of_semester = { 0, 0, 0, 1, 5, ltm->tm_year };   // 1 июня
    }
    else {
        start_of_semester = { 0, 0, 0, 1, 8, ltm->tm_year };  // 1 сентября
        end_of_semester = { 0, 0, 0, 30, 11, ltm->tm_year }; // 30 декабря
    }

    start_semester_time = mktime(&start_of_semester);
    end_semester_time = mktime(&end_of_semester);

    int offset = start_of_semester.tm_wday - 1;
    double offset_diff = offset * 24 * 60 * 60;

    double seconds_diff = difftime(now, start_semester_time) + offset_diff;

    int academic_week = 1 + (seconds_diff / (7 * 24 * 60 * 60));

    return academic_week;
}

int DaysUntilEndOfSemester() {
    time_t now = time(0);
    tm* ltm = localtime(&now);
    tm end_of_semester;

    if (ltm->tm_mon < 8) {
        end_of_semester = { 0, 0, 0, 1, 5, ltm->tm_year };   // 1 июня
    }
    else {
        end_of_semester = { 0, 0, 0, 30, 11, ltm->tm_year }; // 30 декабря
    }

    time_t end_semester_time = mktime(&end_of_semester);
    double seconds_diff = difftime(end_semester_time, now);

    int DaysTillEnd = 2 + seconds_diff / (24 * 60 * 60);

    return DaysTillEnd;
}

int TotalDaysInSemester() {
    time_t now = time(0);
    tm* ltm = localtime(&now);
    tm start_of_semester;
    tm end_of_semester;

    if (ltm->tm_mon < 8) {
        start_of_semester = { 0, 0, 0, 9, 1, ltm->tm_year }; // 9 февраля
        end_of_semester = { 0, 0, 0, 1, 5, ltm->tm_year };   // 1 июня
    }
    else {
        start_of_semester = { 0, 0, 0, 1, 8, ltm->tm_year };  // 1 сентября
        end_of_semester = { 0, 0, 0, 30, 11, ltm->tm_year }; // 30 декабря
    }

    time_t start_semester_time = mktime(&start_of_semester);
    time_t end_semester_time = mktime(&end_of_semester);

    int total_days = difftime(end_semester_time, start_semester_time) / (24 * 60 * 60) + 1;

    return total_days;
}

void Percentage() {
    gotoxy(0, 2);
    cout << "                                                                                                   ";   // Костыль :3
    gotoxy(0, 2);

    int total_days = TotalDaysInSemester();
    int days_passed = total_days - DaysUntilEndOfSemester();
    float done_percentage = (static_cast<float>(days_passed) / total_days) * 100;

    int bar_width = 50;
    int progress = bar_width * done_percentage / 100;

    cout << "Прогресс семестра: [";
    SetColor(32);
    for (int i = 0; i < bar_width; ++i) {
        if (i < progress) cout << "=";
        else cout << " ";
    }
    SetColor(0);
    cout << "] ";
    SetColor(32);
    cout << fixed << setprecision(2) << done_percentage << "%";
    SetColor(0);
    if (done_percentage == 100) cout << " Sheeeeeeeeeeeeeeeeeeeeeeesh ";
    cout << "\n\n";
}

void WeekOfEducation() {
    time_t now = time(0);
    tm* ltm = localtime(&now);
    int academic_week = WeekCalculation();
    int TillEnd = DaysUntilEndOfSemester();

    Percentage();
    cout << "Текущая дата: " << setfill('0') << setw(2) << ltm->tm_mday << "." << setw(2) << 1 + ltm->tm_mon << "." << setw(2) << 1900 + ltm->tm_year;
    cout << "\nНомер учебной недели: " << setfill('0') << setw(2) << academic_week;
    cout << "\nДо конца семестра осталось ";
    if (TillEnd > 0) {
        cout << TillEnd << " ";
        if (TillEnd == 1 || (TillEnd > 20 && TillEnd % 10 == 1)) {
            cout << "день ";
        }
        else if ((TillEnd >= 2 && TillEnd <= 4) || (TillEnd > 20 && (TillEnd % 10 >= 2 && TillEnd % 10 <= 4))) {
            cout << "дня ";
        }
        else {
            cout << "дней ";
        }
    }

    _getch();
}