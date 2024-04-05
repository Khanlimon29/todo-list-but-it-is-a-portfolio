#include "MultTable.h"
#include <iostream>
#include <conio.h>
#include "RandNumb.h"
using namespace std;

void TableTrainer() {
	RandomNumberGenerator rng;
	int quantity;
	int a;
	int b;
	int answer = -1;
	cout << "Выберите количество примеров: ";
	cin >> quantity;
	while (cin.fail()) {
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "Введите значение повторно: ";
		cin >> quantity;
	}
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
	cout << "\nНажмите на любую кнопку для продолжения";
	_getch();


}