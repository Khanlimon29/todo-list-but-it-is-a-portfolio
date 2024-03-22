#include <string>
#include <iostream>
#include <fstream>
#include <conio.h>
#include <Windows.h>
#include <vector>
#include <sstream>

using namespace std;
string options[] = { "1", "2" };

void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void MenuDraw(int NumbOfOpt, int CurrOpt) {

    system("cls");
    for (int i = 0; i < NumbOfOpt; ++i) {
        if (i == CurrOpt) {
            cout << "> ";
        }
        cout << options[i] << endl;
    }

}

struct Todo {
    bool check;
    int number;
    string task;
};

// Функция для чтения данных из файла и заполнения структуры Todo
vector<Todo> readTodoList(const string& filename) {
    vector<Todo> todoList;
    ifstream file(filename);
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            Todo todo;
            stringstream ss(line);
            char check;
            ss >> check >> todo.number; // Читаем первые два числа
            todo.check = (check == '1'); // Преобразуем символ в булевое значение
            getline(ss, todo.task); // Оставшаяся часть строки является заданием
            todoList.push_back(todo);
        }
        file.close();
    }
    else {
        cout << "Unable to open file: " << filename << endl;
    }
    return todoList;
}

// Функция для записи данных из структуры Todo в файл
void writeTodoList(const string& filename, const vector<Todo>& todoList) {
    ofstream file(filename);
    if (file.is_open()) {
        for (const Todo& todo : todoList) {
            // Записываем значения структуры в файл
            file << (todo.check ? "1" : "0") << ". " << todo.number << ". " << todo.task << endl;
        }
        file.close();
    }
    else {
        cout << "Unable to open file: " << filename << endl;
    }
}


int main() {
    const int NumbOfOpt = sizeof(options) / sizeof(options[0]);
    int CurrOpt = 0;
    char key;
    bool running = true;
    MenuDraw(NumbOfOpt, CurrOpt);
    while (running) {
        if (_kbhit()) { // Check if a key is pressed
            key = _getch(); // Get the key
            switch (key) {
            case 72: // Up
                CurrOpt = (CurrOpt - 1 + NumbOfOpt) % NumbOfOpt;
                MenuDraw(NumbOfOpt, CurrOpt);
                break;
            case 80: // Down
                CurrOpt = (CurrOpt + 1) % NumbOfOpt;
                MenuDraw(NumbOfOpt, CurrOpt);
                break;
            case 13: // Enter
                system("cls");
                cout << CurrOpt + 1;
                _getch();
                MenuDraw(NumbOfOpt, CurrOpt);
                break;
            case 27: // Esc
                running = false;
                break;
            }
        }
    }
    string inputFilename = "Todo LIst.txt";
    string outputFilename = "Modified Todo LIst.txt";

    // Читаем данные из исходного файла
    vector<Todo> todoList = readTodoList(inputFilename);

    cout << "Original Todo List:" << endl;
    for (const Todo& todo : todoList) {
        cout << (todo.check ? "1" : "0") << ". " << todo.number << ". " << todo.task << endl;
    }
    cout << endl;

    // Записываем модифицированные данные в другой файл
    writeTodoList(outputFilename, todoList);
    return 0;
}
