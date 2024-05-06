#include "StringFinder.h"
#include <iostream>
#include <conio.h>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

//int searchSubstringFirst(const string& str, const string& substr) {
//    size_t found = str.find(substr);
//    if (found != string::npos) {
//        return found;
//    }
//    return -1;
//}
//
//int searchSubstringAll (const string& str, const string& substr, vector<int>& positions) {
//    size_t found = str.find(substr);
//    int count = 0;
//    while (found != string::npos) {
//        positions.push_back(found);
//        found = str.find(substr, found + 1);
//        count++;
//    }
//    return count;
//}

int searchSubstring(const string& str, const string& substr, vector<int>& positions, bool first) {
    size_t found = str.find(substr);
    if (found != string::npos) {
        positions.push_back(found);
        if (first)
            return 1;
        else {
            int count = 1;
            while (found != string::npos) {
                found = str.find(substr, found + 1);
                if (found != string::npos) {
                    positions.push_back(found);
                    count++;
                }
            }
            return count;
        }
    }
    return 0;
}


void DrawStringMenu(bool first) {
    system("cls");
    cout << "Выбранный вариант: Поиск подстроки: Программа для поиска подстроки в строке и вывода позиции ее первого вхождения\n\n";
    cout << "Текущий режим работы:";
    if (first) cout << "Поиск первого вхождения";
    else cout << "Поиск всех вхождений";
    cout << "\n1. Поиск внутри файла\n2. Поиск внутри строки введённой вручную\nEnter. Смена режима работы (Поиск первого вхождения / Поиск всех вхождений)\n\nEsc. Выход";
}

void StringFinder() {
    bool first = false;
    DrawStringMenu(first);
    bool running = true;
    char choice;
    while (running) {
        choice = _getch();
        switch (choice) {
        case '1': {
            system("cls");
            string filename, substr, line;
            cout << "Введите имя файла: ";
            cin >> filename;
            filename = "String_Finder/" + filename + ".txt";
            cout << "Введите подстроку для поиска: ";
            cin >> substr;

            ifstream file(filename);
            if (file.is_open()) {
                int line_number = 1;
                vector<int> positions;
                while (getline(file, line)) {
                    int count = searchSubstring(line, substr, positions, first);
                    for (int i = 0; i < count; ++i) {
                        cout << "Подстрока найдена в строке " << line_number << ", позиция: " << positions[i] << endl;
                    }
                    line_number++;
                }
                file.close();
            }
            else {
                cout << "Невозможно открыть файл\n";
            }
            _getch();
            DrawStringMenu(first);
            break;
        }
        case '2': {
            system("cls");
            string str, substr;
            cout << "Введите строку: ";
            cin.ignore();
            getline(cin, str);
            cout << "Введите подстроку для поиска: ";
            cin >> substr;

            vector<int> positions;
            int count = searchSubstring(str, substr, positions, first);
            if (count > 0) {
                cout << "Подстрока найдена " << count << " раз(а) в позициях: ";
                for (int i = 0; i < count; ++i) {
                    cout << positions[i];
                    if (i < count - 1) cout << ", ";
                }
                cout << endl;
            }
            else {
                cout << "Подстрока не найдена\n";
            }
            _getch();
            DrawStringMenu(first);
            break;
        }
        case 13: {
            first = !first;
            DrawStringMenu(first);
            break;
        }
        case 27: {
            running = false;
            break;
        }
        }
    }
}
