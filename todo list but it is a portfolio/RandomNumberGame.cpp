#include "RandomNUmberGame.h"
#include "RandNumb.h"
#include <iostream>
#include <conio.h>
using namespace std;

void RandomNumberGame() {
	RandomNumberGenerator rng;
	int UserNumber = -1;
	int RandomNumber = rng.getRandomInRange(0, 10);
	cout << "Я загадал случайное число от 0 до 10, попробуй отгадать чтобы выйти в главное меню\n";
	while (UserNumber != RandomNumber) {
		cout << "Ваше число: ";
		cin >> UserNumber;
		while (cin.fail() || UserNumber < 0 || UserNumber > 10) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Введите значения повторно. Ваше число: ";
			cin >> UserNumber;
		}	
	}
	cout << "Поздравляю, вы угадали число\nНажмите на любую кнопку для продолжения";
	_getch();	
}
