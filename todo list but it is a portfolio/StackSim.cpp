#include <iostream>
#include <conio.h> 
#include <string> 
#include "StackSim.h"
using namespace std;

const int MAX_SIZE = 30;

class Stack {
private:
    int top;
    string data[MAX_SIZE]; 
public:
    Stack() {
        top = -1;
    }

    bool isEmpty() {
        return top == -1;
    }

    bool isFull() {
        return top == MAX_SIZE - 1;
    }

    void push(const string& value) { 
        if (isFull()) {
            cout << "Стек заполнен. Невозможно добавить элемент\n\nДля продолжения нажмите любую кнопку";
            _getch();
            return;
        }
        data[++top] = value;
    }

    void pop() {
        if (isEmpty()) {     
            return;
        }
        top--;
    }

    string peek() { 
        if (isEmpty()) {
            return "";
        }
        return data[top];
    }

    void display() {
        if (isEmpty()) {
            cout << "Стек пуст" << endl;
            return;
        }
        cout << "Содержимое стека: ";
        for (int i = 0; i <= top; ++i) {
            cout << data[i] << " ";
        }
        cout << endl;
    }
};

void StackMenu(Stack stack) {
    system("cls");
    cout << "Выбранный вариант: Симулятор стека: Реализация структуры данных 'стек' с операциями добавления элемента (push), удаления элемента (pop) и чтение головного элемента (peek)\n\n";
    cout << "Нажмите на нужную цифру для управления:\n1. Push\n2. Pop\n3. Peek\n\n";
    stack.display();
    cout << "\nEsc. Выход";
}

void StackSimulator() {
    Stack stack;
    char choice;
    string value;
    bool running = true;
    string topElement;
    StackMenu(stack);
    while (running) {
        choice = _getch();
        switch (choice) {
        case '1': {
            system("cls");
            cout << "Выбранный вариант: Симулятор стека: Реализация структуры данных 'стек' с операциями добавления элемента (push), удаления элемента (pop) и чтение головного элемента (peek)\n\n";
            cout << "Введите строку для добавления в стек: ";
            cin >> value;
            stack.push(value);
            StackMenu(stack);
            break;
        }
        case '2': {
            system("cls");
            stack.pop();
            StackMenu(stack);
            break;
        }
        case '3': {
            system("cls");
            cout << "Выбранный вариант: Симулятор стека: Реализация структуры данных 'стек' с операциями добавления элемента (push), удаления элемента (pop) и чтение головного элемента (peek)\n\n";
            topElement = stack.peek();
            if (!topElement.empty())
                cout << "Головной элемент стека: " << topElement << "\n\nДля продолжения нажмите любую кнопку";
            else
                cout << "Стек пуст\n\nДля продолжения нажмите любую кнопку";
            _getch();
            StackMenu(stack);
            break;
        }
        case 27: {
            running = false;
            break;
        }
        }
    }
}

