#include <iostream>
#include <conio.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "gotoxy.h"

using namespace std;

vector<string> split(const string line, char delimiter) {
    vector<string> tokens;
    string token;
    istringstream tokenStream(line);
    while (getline(tokenStream, token, delimiter)) {
        for (char& c : token) {
            if (c == '_') {
                c = ' ';
            }
        }
        tokens.push_back(token);
    }
    return tokens;
}

void CsvParser() {
    char delimiter;
    string filename;
    string File_Dir_Parc = "Parc_files/";
    cout << "Введите имя файла: ";
    cin >> filename;
    filename = File_Dir_Parc + filename + ".csv";
    ifstream file(filename);

    if (!file.is_open()) {
        cout << "Не удалось открыть файл.\nНажмите на любую кнопку для продолжения";
        _getch();
        return;
    }

    cout << "Выберите разделитель:\n1. ','\n2. ';'\n3. Пробел\n4. Табуляция\n\n";
    char choice;
    choice = _getch();
    switch (choice) {
    case '1': {
        delimiter = ',';
        break;
    }
    case '2': {
        delimiter = ';';
        break;
    }
    case '3': {
        delimiter = ' ';
        break;
    }
    case '4': {
        delimiter = '\t';
        break;
    }
    }

    string line;
    int row = 10;
    while (getline(file, line)) {
        vector<string> tokens = split(line, delimiter);
        int column = 0;
        for (const auto& token : tokens) {
            gotoxy(column * 30, row);
            cout << "|" << token << "\t";
            column++;
        }
        gotoxy(column * 30, row);
        cout << "|" << endl;
        gotoxy(0, row + 1);
        for (int i = 0; i < tokens.size(); ++i) {
            cout << "+-----------------------------";
        }
        cout << "+" << endl;
        row += 2;
    }

    file.close();
    cout << "\n\nНажмите на любую кнопку для продолжения";
    _getch();
}