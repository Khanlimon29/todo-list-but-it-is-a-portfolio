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

class Todo {
public:
    int check;
    string task;

    Todo() : check(0), task("") {} // Первоначальная ининциализация с пустой строки
    Todo(int c, const string& t) : check(c), task(t) {} // Конструктор с параметрами
};

class TodoManager {
public:
    TodoManager(const string& filename) : filename(filename), currOpt(0), running(true), todos(TodoListReadFromFile(filename)/*, numbOfOpt = todo.size()*/)  // Задать параметры по numbOfOpt почему-то не выходит
    {
        DrawMenu(); // Конструктор с параметрами и отрисовкой списка
    }

    void Run() {
        while (running) {
            if (_kbhit()) {
                key = _getch();
                switch (key) {
                    case 72: MoveUp(); break;            // Up            
                    case 80: MoveDown(); break;          // Down
                    case 13: TaskSelection(); break;     // Enter
                    case 83: TaskDelete(); break;        // Delete
                    case 110: TaskAdd(); break;          // N
                    case 101: TaskStatusToggle(); break; // E
                    case 115: TaskSave(); break;         // S
                    case 109: TaskEditCurr(); break;     // M
                    case 122: UndoLastChange(); break;   // Z
                    case 27: ExitProgram(); break;       // ESC
                }                
            }
        }
    }

private:
    string filename;
    vector<Todo> todos;
    stack<vector<Todo>> historyStack;
    int currOpt;
    int numbOfOpt = todos.size();
    char key;
    bool running;

    vector<Todo> TodoListReadFromFile(const string& filename) {
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

    void TodoListWriteToFile(const string& filename) {
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

    void TaskSelection() {
        system("cls");
        cout << "Выбранный вариант: " << todos[currOpt].task << "\n\n";
        if (todos[currOpt].check == 1) {
            cout << "  ***Work in progress***         ";
        }
        else if (todos[currOpt].check == 0) {
            cout << "TODO";
            _getch();
            DrawMenu();
            return;
        }
        switch (currOpt) {
            case 0: cout << "Вся программа является списком задач"; _getch(); break;
            case 1: ASCIIfinder(); break;
            case 2: Calculator(); break;
            case 3: tic_tac(); break;
            case 4: RandomNumber(); break;
            case 5: Generator(); break;
            case 7: RandomNumberGame(); break;
            case 8: Saver(); break;
            case 9: StackSimulator(); break;
            case 10: QueueSimulator(); break;
            case 12: TableTrainer(); break;
            case 14: Finder(); break;
            case 15: Converter(); break;
            case 16: StringFinder(); break;
            case 19: DateFinder(); break;
            case 20: CsvParser(); break;
            case 24: Sorts(); break;
            case 29: WeekOfEducation(); break;
        }
        DrawMenu();
    }

    int Done() {
        int done = 0;
        for (int i = 0; i < numbOfOpt; i++) {
            if (!todos[i].task.compare(0, 11, "Side quest:")) {
                continue;
            }
            if (todos[i].check == 2) done++;
        }
        return done;
    }

    void DrawLine(int i) {
        cout << (i == currOpt ? "> " : "  ");
        switch (todos[i].check) {
            case 0: SetColor(31); cout << "TODO"; break;
            case 1: SetColor(33); cout << "WIP "; break;
            case 2: SetColor(32); cout << "DONE"; break;
        }
        SetColor(0);
        cout << " | " << i << ". ";
        if (i < 10) cout << " ";
        cout << todos[i].task << "\r";
    }

    void DrawPercentage() {
        gotoxy(0, 2);
        cout << string(100, ' ');
        gotoxy(0, 2);
        float donePercentage = 0;
        int totalTasks = 0;
        for (const auto& todo : todos) {
            if (todo.task.compare(0, 11, "Side quest:") == 0) {
                continue;
            }
            totalTasks++;
        }
        if (totalTasks > 0) {
            donePercentage = (static_cast<float>(Done()) / totalTasks) * 100;
        }
        int barWidth = 50;
        int progress = barWidth * donePercentage / 100;
        cout << "Прогресс: [";
        SetColor(32);
        for (int i = 0; i < barWidth; ++i) {
            if (i < progress) cout << "=";
            else cout << " ";
        }
        SetColor(0);
        cout << "] ";
        SetColor(32);
        cout << donePercentage << "%";
        SetColor(0);
        if (donePercentage == 100) cout << " Sheeeeeeeeeeeeeeeeeeeeeeesh ";
        cout << "\n\n";
    }

    void DrawMenu() {
        system("cls");
        cout << "Меню управления: ↑ - строчка вверх, ↓ - строчка вниз, N - создание нового пункта, M - редактирование пункта, E - закрытие пункта, S - сохранение изменений, Z - отмена последнего изменения";
        cout << "\nDelete - удаление пункта, Enter - выбор пункта, Esc - выход из программы\n";

        DrawPercentage();

        for (size_t i = 0; i < todos.size(); ++i) {
            DrawLine(i);
            cout << endl;
        }
        gotoxy(0, currOpt + 4);
    }

    void TaskAdd() {
        system("cls");
        historyStack.push(todos);
        cout << "Введите новый пункт: ";
        string newTask;
        getline(cin, newTask);
        todos.insert(todos.begin() + currOpt, Todo{ 0, newTask });
        numbOfOpt = todos.size();
        DrawMenu();
    }

    void MoveUp() {
        currOpt = (currOpt - 1 + numbOfOpt) % numbOfOpt;
        cout << "\r  ";
        gotoxy(0, currOpt + 4);
        cout << "> \r";
    }

    void MoveDown() {
        currOpt = (currOpt + 1) % numbOfOpt;
        cout << "\r  ";
        gotoxy(0, currOpt + 4);
        cout << "> \r";
    }

    void ExitProgram() {
        gotoxy(0, numbOfOpt + 4);
        running = false;
    }

    void TaskStatusToggle() {
        historyStack.push(todos);
        todos[currOpt].check = (todos[currOpt].check + 1) % 3;
        DrawPercentage();
        gotoxy(0, currOpt + 4);
        DrawLine(currOpt);
    }

    void TaskDelete() {
        if (!todos.empty()) {
            historyStack.push(todos);
            todos.erase(todos.begin() + currOpt);
            numbOfOpt--;
            if (currOpt >= numbOfOpt) currOpt = numbOfOpt - 1;
            DrawMenu();
        }
    }

    void TaskSave() {
        TodoListWriteToFile(filename);
        while (!historyStack.empty()) {
            historyStack.pop();
        }
        system("cls");
        cout << "Изменения сохранены. Нажмите на любую кнопку для продолжения.";
        _getch();
        DrawMenu();
    }

    void TaskEditCurr() {
        system("cls");
        historyStack.push(todos);
        EditTask(todos[currOpt].task);
        DrawMenu();
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

    void UndoLastChange() {
        if (!historyStack.empty()) {
            todos = historyStack.top();
            historyStack.pop();
            numbOfOpt = todos.size();
            DrawMenu();
        }
    }
};

int main() {
    system("mode 650");
    string List_Dir = "Lists/";
    string filename = List_Dir + "Todo.txt";
    TodoManager manager(filename);
    manager.Run();
    return 0;
}