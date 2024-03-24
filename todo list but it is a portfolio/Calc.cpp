#include <iostream>
#include "Calc.h"
using namespace std;

int Calculator() {
	float a = 0;
	float b = 0;
	float devision = 0;

	cout << "Введите два числа через пробел: ";

	cin >> a;
	cin >> b;

	while (cin.fail()) {
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "Введите значения повторно: ";
		cin >> a;
		cin >> b;
	}

	cout << "\nСумма = ";
	cout << a + b;

	cout << "\nРазность = ";
	cout << a - b;

	cout << "\nУмножение = ";
	cout << a * b;

	if (b != 0) {
		cout << "\nДеление = ";
		devision = a / b;
		cout << devision; 
	}
	else { 
		cout << "\nДеление на ноль невозможно";
	}

	cout << "\n\nНажмите на любую кнопку для выхода";
	return 0;
}