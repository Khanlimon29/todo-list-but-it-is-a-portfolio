#include "Sorts.h"
#include "RandNumb.h"
#include <iostream>
#include <vector>
#include <conio.h>
#include <chrono>
#include "CLI_QOL.h"

using namespace std;

vector<int> Array;

void PrintArrayWithColor(const vector<int>& Array, int idx1, int idx2) {
    for (int i = 0; i < Array.size(); i++) {
        if (i == idx1 || i == idx2) {
            SetColor(31);
        }
        cout << Array[i] << " ";
        SetColor(0);
    }
    cout << endl;
}

void QuickSort(vector<int>& Array, int left, int right, bool visual) {
    if (left < right) {
        int Pivot = Array[(left + right) / 2];
        int i = left, j = right;
        while (i <= j) {
            while (Array[i] < Pivot) i++;
            while (Array[j] > Pivot) j--;
            if (i <= j) {
                swap(Array[i], Array[j]);
                if (visual == true) PrintArrayWithColor(Array, i, j);
                i++;
                j--;
            }
        }
        QuickSort(Array, left, j, visual);
        QuickSort(Array, i, right, visual);
    }
}

void BubbleSort(vector<int>& Array, bool visual) {
    int n = Array.size();
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (Array[j] > Array[j + 1]) {
                swap(Array[j], Array[j + 1]);
                if (visual) PrintArrayWithColor(Array, i, j);
            }
        }
    }
}

void Merge(vector<int>& Array, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    vector<int> L(n1), R(n2);
    for (int i = 0; i < n1; i++) L[i] = Array[left + i];
    for (int j = 0; j < n2; j++) R[j] = Array[mid + 1 + j];
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            Array[k] = L[i];
            i++;
        }
        else {
            Array[k] = R[j];
            j++;
        }
        k++;
    }
    while (i < n1) {
        Array[k] = L[i];
        i++;
        k++;
    }
    while (j < n2) {
        Array[k] = R[j];
        j++;
        k++;
    }
}

void MergeSort(vector<int>& Array, int left, int right, bool visual) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        MergeSort(Array, left, mid, visual);
        MergeSort(Array, mid + 1, right, visual);
        Merge(Array, left, mid, right);
        if (visual) PrintArrayWithColor(Array, left, right);
    }
}

void InsertionSort(vector<int>& Array, bool visual) {
    int n = Array.size();
    for (int i = 1; i < n; i++) {
        int key = Array[i];
        int j = i - 1;
        while (j >= 0 && Array[j] > key) {
            Array[j + 1] = Array[j];
            j--;
            if (visual) PrintArrayWithColor(Array, i, j);
        }
        Array[j + 1] = key;
    }
}

void ShellSort(vector<int>& Array, bool visual) {
    int n = Array.size();
    for (int gap = n / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < n; i++) {
            int temp = Array[i];
            int j;
            for (j = i; j >= gap && Array[j - gap] > temp; j -= gap) {
                Array[j] = Array[j - gap];
                if (visual) PrintArrayWithColor(Array, i, j);
            }
            Array[j] = temp;
        }
    }
}

void SelectionSort(vector<int>& Array, bool visual) {
    int n = Array.size();
    for (int i = 0; i < n - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < n; j++) {
            if (Array[j] < Array[min_idx]) {
                min_idx = j;
                if (visual) PrintArrayWithColor(Array, i, j);
            }
        }
        swap(Array[i], Array[min_idx]);
    }
}

vector<int> RandomArray(int Size) {
    RandomNumberGenerator rng;
    vector<int> newArray;
    for (int i = 0; i < Size; i++) {
        newArray.push_back(rng.getRandomInRange(0, 100));
    }
    return newArray;
}

void PrintArray(vector<int>& Array) {
    for (int i = 0; i < Array.size(); i++) {
        cout << Array[i] << " ";
    }
    cout << "\n";
}

void SortMenu(vector<int>& Array) {
    system("cls");
    cout << "Выбранный вариант: Программа для сортировки массива: Реализация различных алгоритмов сортировки массива, таких как сортировка пузырьком, сортировка выбором и т. д.\n\n";
    cout << "Массив: ";
    if (Array.size() == 0) cout << "Пусто";
    else PrintArray(Array);
    cout << "\nНажмите на нужную цифру для управления:\n1. Быстрая сортировка\n2. Сортировка пузырьком\n3. Сортировка слиянием\n4. Сортировка вставками\n5. Сортировка Шелла\n6. Сортировка выбором\n\nEnter. Задать массив из рандомных чисел (при массиве из менее чем 20 элементов будут выведены этапы сортировки иначе будет замерена скорость выполнения)\nEsc. Выход";
}

void Sorts() {
    cout << "Массив: ";
    if (Array.size() == 0) cout << "Пусто";
    else PrintArray(Array);
    cout << "\nНажмите на нужную цифру для управления:\n1. Быстрая сортировка\n2. Сортировка пузырьком\n3. Сортировка слиянием\n4. Сортировка вставками\n5. Сортировка Шелла\n6. Сортировка выбором\n\nEnter. Задать массив из рандомных чисел (при массиве из менее чем 20 элементов будут выведены этапы сортировки иначе будет замерена скорость выполнения)\nEsc. Выход";
    bool running = true;
    char choice;
    double time = 0;
    bool visual;
    while (running) {
        choice = _getch();
        switch (choice) {
        case '1': {
            system("cls");
            if (Array.size() >= 20) {
                visual = false;
                auto start = chrono::high_resolution_clock::now();
                QuickSort(Array, 0, Array.size() - 1, visual);
                auto end = chrono::high_resolution_clock::now();
                auto duration = chrono::duration_cast<chrono::nanoseconds>(end - start);
                SortMenu(Array);
                cout << "\n\nВремя выполнения быстрой сортировки: " << duration.count() / 1e6 << " мс" << endl;
            }
            else {
                visual = true;
                cout << "Этапы выполнения сортировки:\n";
                PrintArray(Array);
                QuickSort(Array, 0, Array.size() - 1, visual);
                cout << "\nНажмите любую кнопку для продолжения";
                _getch();
                SortMenu(Array);
            }
            break;
        }
        case '2': {
            system("cls");
            if (Array.size() >= 20) {
                visual = false;
                auto start = chrono::high_resolution_clock::now();
                BubbleSort(Array, visual);
                auto end = chrono::high_resolution_clock::now();
                auto duration = chrono::duration_cast<chrono::nanoseconds>(end - start);
                SortMenu(Array);
                cout << "\n\nВремя выполнения сортировки пузырьком: " << time << " мс" << endl;
            }
            else {
                visual = true;
                cout << "Этапы выполнения сортировки:\n";
                PrintArray(Array);
                BubbleSort(Array, visual);
                cout << "\nНажмите любую кнопку для продолжения";
                _getch();
                SortMenu(Array);
            }
            break;
        }
        case '3': {
            system("cls");
            if (Array.size() >= 20) {
                visual = false;
                auto start = chrono::high_resolution_clock::now();
                MergeSort(Array, 0, Array.size() - 1, visual);
                auto end = chrono::high_resolution_clock::now();
                auto duration = chrono::duration_cast<chrono::nanoseconds>(end - start);
                SortMenu(Array);
                cout << "\n\nВремя выполнения сортировки слиянием: " << duration.count() / 1e6 << " мс" << endl;
            }
            else {
                visual = true;
                cout << "Этапы выполнения сортировки:\n";
                PrintArray(Array);
                MergeSort(Array, 0, Array.size() - 1, visual);
                cout << "\nНажмите любую кнопку для продолжения";
                _getch();
                SortMenu(Array);
            }
            break;
        }
        case '4': {
            system("cls");
            if (Array.size() >= 20) {
                visual = false;
                auto start = chrono::high_resolution_clock::now();
                InsertionSort(Array, visual);
                auto end = chrono::high_resolution_clock::now();
                auto duration = chrono::duration_cast<chrono::nanoseconds>(end - start);
                SortMenu(Array);
                cout << "\n\nВремя выполнения сортировки вставками: " << time << " мс" << endl;
            }
            else {
                visual = true;
                cout << "Этапы выполнения сортировки:\n";
                PrintArray(Array);
                InsertionSort(Array, visual);
                cout << "\nНажмите любую кнопку для продолжения";
                _getch();
                SortMenu(Array);
            }
            break;
        }
        case '5': {
            system("cls");
            if (Array.size() >= 20) {
                visual = false;
                auto start = chrono::high_resolution_clock::now();
                ShellSort(Array, visual);
                auto end = chrono::high_resolution_clock::now();
                auto duration = chrono::duration_cast<chrono::nanoseconds>(end - start);
                SortMenu(Array);
                cout << "\n\nВремя выполнения сортировки Шелла: " << time << " мс" << endl;
            }
            else {
                visual = true;
                cout << "Этапы выполнения сортировки:\n";
                PrintArray(Array);
                ShellSort(Array, visual);
                cout << "\nНажмите любую кнопку для продолжения";
                _getch();
                SortMenu(Array);
            }
            break;
        }
        case '6': {
            system("cls");
            if (Array.size() >= 20) {
                visual = false;
                auto start = chrono::high_resolution_clock::now();
                SelectionSort(Array, visual);
                auto end = chrono::high_resolution_clock::now();
                auto duration = chrono::duration_cast<chrono::nanoseconds>(end - start);
                SortMenu(Array);
                cout << "\n\nВремя выполнения сортировки выбором: " << time << " мс" << endl;
            }
            else {
                visual = true;
                cout << "Этапы выполнения сортировки:\n";
                PrintArray(Array);
                SelectionSort(Array, visual);
                cout << "\nНажмите любую кнопку для продолжения";
                _getch();
                SortMenu(Array);
            }
            break;
        }
        case 13: {
            system("cls");
            int Size = 0;
            cout << "Введите размер массива: ";
            cin >> Size;
            while (cin.fail() || Size < 0) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Введите значения повторно: ";
                cin >> Size;
            }
            Array = RandomArray(Size);
            SortMenu(Array);
            break;
        }
        case 27: {
            running = false;
            break;
        }
        }
    }
}
