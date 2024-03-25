#pragma once

#ifndef MAIN_H
#define MAIN_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <Windows.h>
#include <conio.h>
#include <sstream>
#include "Ascii_Finder.h"
#include "Calc.h"
#include "tic_tac_toe.h"

using namespace std;

// Структура для хранения задач
struct Todo {
    bool check;
    string task;
};

// Функция для перемещения курсора
void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// Функция для чтения списка задач из файла
vector<Todo> ReadTodoListFromFile(string filename) {
    vector<Todo> todos;
    ifstream file(filename);
    if (!file) {
        cout << "Ошибка при открытии файла" << endl;
        return todos;
    }

    string line;
    while (getline(file, line)) {
        if (!line.empty()) {
            Todo todo;
            stringstream ss(line);
            ss >> todo.check;
            getline(ss, todo.task);
            todos.push_back(todo);
        }
    }
    return todos;
}

// Функция для записи списка задач в файл
void WriteTodoListToFile(string filename, vector<Todo> todos) {
    ofstream file(filename);
    if (!file) {
        cout << "Ошибка при открытии файла для записи" << endl;
        return;
    }

    for (auto todo : todos) {
        file << todo.check << todo.task << endl;
    }

    file.close();
}

// Функция для отрисовки меню
void MenuDraw(vector<Todo> todos, int currOpt) {
    system("cls");
    cout << "Меню управления: ↑ - строчка вверх, ↓ - строчка вниз, N - создание нового пункта, E - закрытие пункта, S - сохранение изменений, Delete - удаление строки, Enter - выбор пункта, ";
    cout << "\nEsc - выход из программы\n\n";
    for (size_t i = 0; i < todos.size(); ++i) {
        if (i == size_t(currOpt)) cout << "> ";
        else cout << "  ";
        if (todos[i].check == 0) cout << "TODO | ";
        else cout << "DONE | ";
        cout << i << ". ";
        cout << todos[i].task << endl;
    }
    gotoxy(0, currOpt + 3);
}


#endif