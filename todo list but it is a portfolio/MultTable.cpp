#include "MultTable.h"
#include <iostream>
#include <conio.h>
#include "RandNumb.h"
#include <chrono>
#include <iomanip>
using namespace std;

void TableTrainer() {
	RandomNumberGenerator rng;
	int quantity;
	int a;
	int b;
	int answer = -1;
	cout << "Выберите количество примеров: ";
	cin >> quantity;
	while (cin.fail() || quantity <= 0) {
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "Введите значение повторно: ";
		cin >> quantity;
	}
	auto start = chrono::high_resolution_clock::now();
	for (int i = 0; i < quantity; i++) {
		a = rng.getRandomInRange(1, 10);
		b = rng.getRandomInRange(1, 10);
		while (answer != a * b) {
			cout << a << " * " << b << " = ";
			cin >> answer;
			while (cin.fail()) {
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "Введите значение повторно: ";
				cin >> answer;
			}
			if (answer != a * b) cout << "Неверно\n";
		}
		cout << "Верно\n";
	}
	auto end = chrono::high_resolution_clock::now();
	auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
	auto Avarageduration = duration.count() / quantity;

	int Minutes = duration.count() / 60000;
	int Seconds = (duration.count() % 60000) / 1000;
	int Milliseconds = duration.count() % 1000;

	int AvarageSeconds = Avarageduration % 60000 / 1000;
	int AvarageMilliseconds = Avarageduration % 1000;

	cout << "\nЗатраченное время " << Minutes << ":" << setw(2) << setfill('0') << Seconds << ":" << setw(3) << setfill('0') << Milliseconds << " минут\n";
	cout << "На один пример в среднем уходит " << AvarageSeconds << "." << setw(3) << setfill('0') << AvarageMilliseconds << " секунд\n";

	cout << "\nНажмите на любую кнопку для продолжения";
	_getch();
}