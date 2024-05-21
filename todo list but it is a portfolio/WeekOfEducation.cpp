#include "WeekOfEducation.h"
#include <iostream>
#include <conio.h>
#include <ctime>
#include <iomanip>
#include "gotoxy.h"
#include "setcolor.h"

using namespace std;

tm GetSemesterStart(bool isSpring) {
    tm start_of_semester;
    if (isSpring) {
        start_of_semester = { 0, 0, 0, 9, 1, 0 }; // 9 февраля
    }
    else {
        start_of_semester = { 0, 0, 0, 1, 8, 0 }; // 1 сентября
    }
    return start_of_semester;
}

tm GetSemesterEnd(bool isSpring) {
    tm end_of_semester;
    if (isSpring) {
        end_of_semester = { 0, 0, 0, 1, 5, 0 }; // 1 июня
    }
    else {
        end_of_semester = { 0, 0, 0, 30, 11, 0 }; // 30 декабря
    }
    return end_of_semester;
}

time_t GetTimeFromTm(tm& date, int year) {
    date.tm_year = year - 1900;
    return mktime(&date);
}

bool IsSpringSemester(tm* ltm) {
    return ltm->tm_mon < 8;
}

int WeekCalculation() {
    time_t now = time(0);
    tm* ltm = localtime(&now);
    bool isSpring = IsSpringSemester(ltm);

    tm start_of_semester = GetSemesterStart(isSpring);
    time_t start_semester_time = GetTimeFromTm(start_of_semester, ltm->tm_year + 1900);

    int offset = start_of_semester.tm_wday - 1;
    double offset_diff = offset * 24 * 60 * 60;
    double seconds_diff = difftime(now, start_semester_time) + offset_diff;

    return 1 + (seconds_diff / (7 * 24 * 60 * 60));
}

int DaysUntilEndOfSemester() {
    time_t now = time(0);
    tm* ltm = localtime(&now);
    bool isSpring = IsSpringSemester(ltm);

    tm end_of_semester = GetSemesterEnd(isSpring);
    time_t end_semester_time = GetTimeFromTm(end_of_semester, ltm->tm_year + 1900);
    double seconds_diff = difftime(end_semester_time, now);

    return 2 + seconds_diff / (24 * 60 * 60);
}

int TotalDaysInSemester() {
    time_t now = time(0);
    tm* ltm = localtime(&now);
    bool isSpring = IsSpringSemester(ltm);

    tm start_of_semester = GetSemesterStart(isSpring);
    tm end_of_semester = GetSemesterEnd(isSpring);

    time_t start_semester_time = GetTimeFromTm(start_of_semester, ltm->tm_year + 1900);
    time_t end_semester_time = GetTimeFromTm(end_of_semester, ltm->tm_year + 1900);

    return difftime(end_semester_time, start_semester_time) / (24 * 60 * 60) + 1;
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
    cout << "Текущая дата: " << setfill('0') << setw(2) << ltm->tm_mday << "." << setw(2) << 1 + ltm->tm_mon << "." << setw(4) << 1900 + ltm->tm_year;
    cout << "\nНомер учебной недели: " << setfill('0') << setw(2) << academic_week;
    cout << "\nДо конца семестра осталось " << TillEnd << " ";
    if (TillEnd > 0) {
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
