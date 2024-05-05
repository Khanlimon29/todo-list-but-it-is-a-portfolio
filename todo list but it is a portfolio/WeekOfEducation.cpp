#include "WeekOfEducation.h"
#include <iostream>
#include <conio.h>
#include <ctime>
#include <iomanip>

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

void WeekOfEducation() {	
    time_t now = time(0);
    tm* ltm = localtime(&now);
    int academic_week = WeekCalculation();
    int TillEnd = DaysUntilEndOfSemester();

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