#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stack>
#include <Windows.h>
#include <conio.h>
#include <sstream>
#include "Ascii_Finder.h"
#include "Calc.h"
#include "tic_tac_toe.h"
#include "RandNumb.h"
#include "PasswordGen.h"
#include "PrimeNumb.h"

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

void LineDraw(vector<Todo> todos, int currOpt, int i) {
    if (i == size_t(currOpt)) cout << "> ";
    else cout << "  ";
    if (todos[i].check == 0) cout << "TODO | ";
    else cout << "DONE | ";
    cout << i << ".";
    if (i < 10) cout << "  ";
    else cout << " ";
    cout << todos[i].task << "\r";
}

// Функция для отрисовки меню
void MenuDraw(vector<Todo> todos, int currOpt) {
    system("cls");
    cout << "Меню управления: ↑ - строчка вверх, ↓ - строчка вниз, N - создание нового пункта, M - редактирование пункта, E - закрытие пункта, S - сохранение изменений, Z - отмена последнего изменения";
    cout << "\nDelete - удаление пункта, Enter - выбор пункта, Esc - выход из программы\n\n";
    for (size_t i = 0; i < todos.size(); ++i) {
        LineDraw(todos, currOpt, i);
        cout << endl;
    }
    gotoxy(0, currOpt + 3);
}



void NewTask(vector<Todo>& todos, int& numbOfOpt, int& currOpt, const string& newTask) {
    Todo newTodo;
    newTodo.check = false;
    newTodo.task = newTask;
    todos.insert(todos.begin() + currOpt, newTodo);
    numbOfOpt = todos.size();
}

void EditTask(string& task) {
    cout << "Enter - сохранение, Esc - выход без изменений\nРедактирование: ";
    string editedTask = task;
    cout << editedTask;
    char ch;
    while ((ch = _getch()) != 13 && ch != 27) { // Enter or Esc
        if (ch == 8) { // Backspace 
            if (!editedTask.empty()) {
                editedTask.pop_back();
                cout << "\b \b";
            }
        }
        else {
            editedTask.push_back(ch);
            cout << ch;
        }
    }
    if (ch == 13) // Enter save
        task = editedTask;
}

int main() {
    system("mode 650");
    string filename = "Todo.txt";
    vector<Todo> todos = ReadTodoListFromFile(filename);
    stack<vector<Todo>> historyStack;

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
                cout << "\r  ";
                gotoxy(0, currOpt + 3);
                cout << "> \r";
                break;
            }
            case 80: // Down
            {
                currOpt = (currOpt + 1) % numbOfOpt;
                cout << "\r  ";
                gotoxy(0, currOpt + 3);
                cout << "> \r";
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
                if (currOpt == 4) {
                    RandomNumber();
                }
                if (currOpt == 5) {
                    Generator();
                }
                if (currOpt == 15) {
                    Finder();
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
                historyStack.push(todos); 
                cout << "Введите новый пункт: ";
                string newTask;
                getline(cin, newTask);
                NewTask(todos, numbOfOpt, currOpt, newTask);
                MenuDraw(todos, currOpt);
                break;
            }
            case 101: // E
            {
                historyStack.push(todos); 
                todos[currOpt].check = !todos[currOpt].check;
                LineDraw(todos, currOpt, currOpt);
                break;
            }
            case 83: // Delete
            {
                if (!todos.empty()) {
                    historyStack.push(todos);
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
                while (!historyStack.empty()) {
                    historyStack.pop();
                }
                system("cls");
                cout << "Изменения сохранены. Нажмите на любую кнопку для продолжения.";
                _getch();
                MenuDraw(todos, currOpt);
                break;
            }
            case 109: // M
            {
                system("cls");
                historyStack.push(todos);
                EditTask(todos[currOpt].task);
                MenuDraw(todos, currOpt);
                break;
            }
            case 122: // Z
            {
                if (!historyStack.empty()) {
                    todos = historyStack.top();
                    historyStack.pop();
                    numbOfOpt = todos.size();
                    MenuDraw(todos, currOpt);
                }
                break;
            }
            }
        }
    }
    return 0;
}