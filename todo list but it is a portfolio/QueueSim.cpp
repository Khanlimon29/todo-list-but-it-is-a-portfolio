#include <iostream>
#include <conio.h> 
#include <string> 
#include "QueueSim.h"
using namespace std;

const int MAX_SIZE = 30;

class Queue {
private:
    int front, rear;
    string data[MAX_SIZE];
public:
    Queue() {
        front = -1;
        rear = -1;
    }

    bool isEmpty() {
        return front == -1 && rear == -1;
    }

    bool isFull() {
        return (rear + 1) % MAX_SIZE == front;
    }

    void enqueue(const string& value) {
        if (isFull()) {
            cout << "Очередь заполнена. Невозможно добавить элемент\n\nДля продолжения нажмите любую кнопку";
            _getch();
            return;
        }
        if (isEmpty()) {
            front = 0;
            rear = 0;
        }
        else {
            rear = (rear + 1) % MAX_SIZE;
        }
        data[rear] = value;
    }

    void dequeue() {
        if (isEmpty()) {
            return;
        }
        else if (front == rear) {
            front = -1;
            rear = -1;
        }
        else {
            front = (front + 1) % MAX_SIZE;
        }
    }

    void display() {
        if (isEmpty()) {
            cout << "Очередь пуста" << endl;
            return;
        }
        cout << "Содержимое очереди: ";
        int i = front;
        while (i != rear) {
            cout << data[i] << " ";
            i = (i + 1) % MAX_SIZE;
        }
        cout << data[rear] << endl;
    }
};

void QueueMenu(Queue queue) {
    system("cls");
    cout << "Выбранный вариант: Симулятор очереди: Реализация структуры данных 'очередь' с операциями добавления элемента (enqueue) и извлечения элемента (dequeue)\n\n";
    cout << "Нажмите на нужную цифру для управления:\n1. Enqueue\n2. Dequeue\n\n";
    queue.display();
    cout << "\nEsc. Выход";
}

void QueueSimulator() {
    Queue queue;
    char choice;
    string value;
    bool running = true;
    string frontElement;
    QueueMenu(queue);
    while (running) {
        choice = _getch();
        switch (choice) {
        case '1': {
            system("cls");
            cout << "Выбранный вариант: Симулятор очереди: Реализация структуры данных 'очередь' с операциями добавления элемента (enqueue) и извлечения элемента (dequeue)\n\n";
            cout << "Введите строку для добавления в очередь: ";
            cin >> value;
            queue.enqueue(value);
            QueueMenu(queue);
            break;
        }
        case '2': {
            system("cls");
            queue.dequeue();
            QueueMenu(queue);
            break;
        }
        case 27: {
            running = false;
            break;
        }
        }
    }
}
