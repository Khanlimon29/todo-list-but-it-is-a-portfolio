//TODO: Поддержка киррилицы, у пользователя выводятся символы явно не из кириллицы лол
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stack>
#include <conio.h>
#include <sstream>
#include "gotoxy.h"
#include "Ascii_Finder.h"
#include "Calc.h"
#include "tic_tac_toe.h"
#include "RandNumb.h"
#include "PasswordGen.h"
#include "PrimeNumb.h"
#include "RandomNumberGame.h"
#include "StackSim.h"
#include "QueueSim.h"
#include "ScreenSaver.h"
#include "MultTable.h"
#include "setcolor.h"
#include "Sorts.h"
#include "Date.h"
#include "Converter.h"
#include "CsvPars.h"
#include "WeekOfEducation.h"
#include "StringFinder.h"

using namespace std;
// Структура для хранения задач
struct Todo {
    int check;
    string task;
};

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

// Функция для подсчёта выполненных пунктов
int Done(vector<Todo> todos, int NumbOfOpt) {
    int done = 0;
    for (int i = 0; i < NumbOfOpt; i++) {
        if (!todos[i].task.compare(0, 11, "Side quest:")) {
            continue;
        }
        if (todos[i].check == 2) done++;
    }
    return done;
}

// Функция для отрисовки одной строчки
void LineDraw(vector<Todo> todos, int currOpt, int i) {

    if (i == size_t(currOpt)) cout << "> ";
    else cout << "  ";

    if (todos[i].check == 0) {
        SetColor(31);
        cout << "TODO";         
    }
    else if (todos[i].check == 1) {
        SetColor(33);
        cout << "WIP ";
    }
    else if (todos[i].check == 2) {
        SetColor(32);
        cout << "DONE";
    }
    SetColor(0);

    cout << " | ";
    cout << i << ".";
    if (i < 10) cout << "  ";
    else cout << " ";
    cout << todos[i].task << "\r";
}

// Функция для отрисовки процента выполненных заданий и шкалы прогресса
void PercentageDraw(vector<Todo> todos, int numbOfOpt) {
    gotoxy(0, 2);
    cout << "                                                                                                   ";   // Костыль :3
    gotoxy(0, 2);
    float DonePercentage = 0;
    int totalTasks = 0;
    for (size_t i = 0; i < todos.size(); ++i) {
        if (!todos[i].task.compare(0, 11, "Side quest:")) {
            continue; 
        }
        totalTasks++;
    }
    if (totalTasks > 0) {
        DonePercentage = (static_cast<float>(Done(todos, numbOfOpt)) / totalTasks) * 100;
    }
    int barWidth = 50;
    int progress = barWidth * DonePercentage / 100;
    cout << "Прогресс: [";
    SetColor(32);
    for (int i = 0; i < barWidth; ++i) {
        if (i < progress) cout << "=";
        else cout << " ";
    }
    SetColor(0);
    cout << "] ";
    SetColor(32);
    cout << DonePercentage << "%";
    SetColor(0);
    if (DonePercentage == 100) cout << " Sheeeeeeeeeeeeeeeeeeeeeeesh ";
    cout << "\n\n";
}

// Функция для отрисовки меню
void MenuDraw(vector<Todo> todos, int currOpt, int numbOfOpt) {
    system("cls");
    cout << "Меню управления: ↑ - строчка вверх, ↓ - строчка вниз, N - создание нового пункта, M - редактирование пункта, E - закрытие пункта, S - сохранение изменений, Z - отмена последнего изменения";
    cout << "\nDelete - удаление пункта, Enter - выбор пункта, Esc - выход из программы\n";

    PercentageDraw(todos, numbOfOpt);

    for (size_t i = 0; i < todos.size(); ++i) {
        LineDraw(todos, currOpt, i);
        cout << endl;
    }
    gotoxy(0, currOpt + 4);
}

// Функция для добавления нового пункта
void NewTask(vector<Todo>& todos, int& numbOfOpt, int& currOpt, const string& newTask) {
    Todo newTodo;
    newTodo.check = 0;
    newTodo.task = newTask;
    todos.insert(todos.begin() + currOpt, newTodo);
    numbOfOpt = todos.size();
}

// Функция для редактирования пункта
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
    string List_Dir = "Lists/";
    string filename = List_Dir + "Todo.txt";
    int currOpt = 0;
    char key;
    bool running = true;
    vector<Todo> todos = ReadTodoListFromFile(filename);
    stack<vector<Todo>> historyStack;
    int numbOfOpt = todos.size();
    MenuDraw(todos, currOpt, numbOfOpt);

    int checkCounter = 0;

    while (running) {
        if (_kbhit()) {
            key = _getch();
            switch (key) {
            case 72: // Up
            {
                currOpt = (currOpt - 1 + numbOfOpt) % numbOfOpt;
                cout << "\r  ";
                gotoxy(0, currOpt + 4);
                cout << "> \r";
                break;
            }
            case 80: // Down
            {
                currOpt = (currOpt + 1) % numbOfOpt;
                cout << "\r  ";
                gotoxy(0, currOpt + 4);
                cout << "> \r";
                break;
            }
            case 13: // Enter
            {
                system("cls");
                cout << "Выбранный вариант: " << todos[currOpt].task << "\n\n";
                if (todos[currOpt].check == 1) {
                    cout << "  ***Work in progress***         ";
                }
                else if (todos[currOpt].check == 0) {
                    cout << "TODO";
                    _getch();
                    MenuDraw(todos, currOpt, numbOfOpt);
                    break;
                }
                switch (currOpt) {
                case 0: {
                    cout << "Вся программа является списком задач";
                    _getch();
                    break;
                }
                case 1: {
                    ASCIIfinder();
                    break;
                }
                case 2: {
                    Calculator();
                    break;
                }
                case 3: {
                    tic_tac();
                    break;
                }
                case 4: {
                    RandomNumber();
                    break;
                }
                case 5: {
                    Generator();
                    break;
                }
                case 7: {
                    RandomNumberGame();
                    break;
                }
                case 8: {
                    Saver();
                    break;
                }
                case 9: {
                    StackSimulator();
                    break;
                }
                case 10: {
                    QueueSimulator();
                    break;
                }
                case 12: {
                    TableTrainer();
                    break;
                }
                case 14: {
                    Finder();
                    break;
                }
                case 15: {
                    Converter();
                    break;
                }
                case 16: {
                    StringFinder();
                    break;
                }
                case 19: {
                    DateFinder();
                    break;
                }
                case 20: {
                    CsvParser();
                    break;
                }
                case 24: {
                    Sorts();
                    break;
                }
                case 29: {
                    WeekOfEducation();  // TODO: оптимизация надо очень
                    break;
                }
                }
                MenuDraw(todos, currOpt, numbOfOpt);
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
                MenuDraw(todos, currOpt, numbOfOpt);
                break;
            }
            case 101: // E
            {
                historyStack.push(todos);
                todos[currOpt].check = (todos[currOpt].check + 1) % 3;
                PercentageDraw(todos, numbOfOpt);
                gotoxy(0, currOpt + 4);
                LineDraw(todos, currOpt, currOpt);
                break;
            }
            case 83: // Delete
            {
                if (!todos.empty()) {
                    historyStack.push(todos);
                    todos.erase(todos.begin() + currOpt);
                    numbOfOpt--;
                    if (currOpt >= numbOfOpt) currOpt = numbOfOpt - 1;
                    MenuDraw(todos, currOpt, numbOfOpt);
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
                MenuDraw(todos, currOpt, numbOfOpt);
                break;
            }
            case 109: // M
            {
                system("cls");
                historyStack.push(todos);
                EditTask(todos[currOpt].task);
                MenuDraw(todos, currOpt, numbOfOpt);
                break;
            }
            case 122: // Z
            {
                if (!historyStack.empty()) {
                    todos = historyStack.top();
                    historyStack.pop();
                    numbOfOpt = todos.size();
                    MenuDraw(todos, currOpt, numbOfOpt);
                }
                break;
            }
            }
        }
    }
    return 0;
}
