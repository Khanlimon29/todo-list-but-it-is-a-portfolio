#include "StringFinder.h"
#include <iostream>
#include <conio.h>
#include <fstream>
#include <string>
#include <vector>
 #include <sstream>

using namespace std;

int searchSubstring(const string& str, const string& substr, vector<int>& positions, bool first) {
    stringstream ss(str);
    string word;
    int word_number = 1; 
    int count = 0;
    while (ss >> word) {
        size_t found = word.find(substr);
        if (found != string::npos) {
            positions.push_back(word_number);
            count++;
            if (first) return 1;
        }
        word_number++;
    }
    return count;
}

void DrawStringMenu(bool first) {
    system("cls");
    cout << "Выбранный вариант: Поиск подстроки: Программа для поиска подстроки в строке и вывода позиции ее первого вхождения\n\n";
    cout << "Текущий режим работы: ";
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
                if (!first) cout << "Подстрока найдена " << count << " раз(а), в словах с номерами ";
                else cout << "Подстрока найдена в слове с номером ";
                for (int i = 0; i < count; ++i) {
                    cout << positions[i];
                    if (i < count - 1) cout << ", ";
                    else cout << " ";
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
