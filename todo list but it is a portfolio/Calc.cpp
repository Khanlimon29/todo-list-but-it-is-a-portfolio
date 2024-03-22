#include <iostream>
#include "Calc.h"
using namespace std;

int Calculator() {
	float a = 0;
	float b = 0;
	float devision = 0;

	cout << "Введите два числа:";
	cin >> a;
	cin >> b;

	cout << "Сумма = ";
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

	cout << "\nНажмите на любую кнопку для выхода";
	return 0;
}