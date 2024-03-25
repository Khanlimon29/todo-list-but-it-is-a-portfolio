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

int main() {
    system("mode 650");
    string filename = "Todo.txt";
    vector<Todo> todos = ReadTodoListFromFile(filename);

    int numbOfOpt = todos.size();
    int currOpt = 0;
    char key;
    bool running = true;
    MenuDraw(todos, currOpt);

    while (running) {
        if (_kbhit()) {
            key = _getch();
            switch (key) {
            case 72: // Up
            {
                currOpt = (currOpt - 1 + numbOfOpt) % numbOfOpt;
                MenuDraw(todos, currOpt);
                break;
            }
            case 80: // Down
            {
                currOpt = (currOpt + 1) % numbOfOpt;
                MenuDraw(todos, currOpt);
                break;
            }
            case 13: // Enter
            {
                system("cls");
                cout << "Выбранный вариант: " << todos[currOpt].task <<  "\n\n";
                if (todos[currOpt].check == 0) {
                    cout << "TODO";
                    _getch();
                    MenuDraw(todos, currOpt);
                    break;
                }
                if (currOpt == 0) {
                    cout << "Вся программа является списком задач";
                    _getch();
                }
                if (currOpt == 1) {
                    ASCIIfinder();
                }
                if (currOpt == 2) {
                    Calculator();
                }
                if (currOpt == 3) {
                    tic_tac();
                }
                MenuDraw(todos, currOpt);
                break;
            }
            case 27: // Esc
            {
                gotoxy(0, numbOfOpt + 4);
                running = false;
                break;
            }
            case 110: // N
            {
                system("cls");
                cout << "Введите новый пункт: ";
                string newTask;
                getline(cin, newTask);
                Todo newTodo;
                newTodo.check = false;
                newTodo.task = newTask;
                todos.push_back(newTodo);
                numbOfOpt++;
                MenuDraw(todos, currOpt);
                break;
            }
            case 101: // E
            {
                todos[currOpt].check = !todos[currOpt].check;
                MenuDraw(todos, currOpt);
                break;
            }
            case 83: // Delete 
            {
                if (!todos.empty()) {
                    todos.erase(todos.begin() + currOpt);
                    numbOfOpt--;
                    if (currOpt >= numbOfOpt)
                        currOpt = numbOfOpt - 1;
                    MenuDraw(todos, currOpt);
                }
                break;
            }
            case 115: // S
            {
                WriteTodoListToFile(filename, todos);
                system("cls");
                cout << "Изменения сохранены. Нажмите на любую кнопку для продолжения.";
                _getch();
                MenuDraw(todos, currOpt);
                break;
            }
            }
        }
    }
    return 0;
}
