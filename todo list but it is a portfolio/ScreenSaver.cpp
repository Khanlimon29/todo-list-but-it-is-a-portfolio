#include "ScreenSaver.h"
#include <iostream>
#include <conio.h>
#include "CLI_QOL.h"
#include "RandNumb.h"
#include <windows.h>

using namespace std;

int OffsetX = 1;
int OffsetY = 2;
int ScreenSizeX = 150;
int ScreenSizeY = 30;
int LogoSizeX = 30;
int LogoSizeY = 7;

void DrawScreen() {
	cout << "\n";
	for (int i = 0; i <= ScreenSizeX + 1; i++) {
		cout << "#";
	}
	for (int i = 0; i < ScreenSizeY - 1; i++) {
		cout << "\n#";
		for (int j = 0; j < ScreenSizeX; j++) {
			cout << " ";
		}
		cout << "#";
	}
	cout << "\n";
	for (int i = 0; i <= ScreenSizeX + 1; i++) {
		cout << "#";
	}
}

void DrawLogo(int x, int y) {
	
	gotoxy(x, y + OffsetY);
	cout << " ⠀⣸⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠀⠀⠀⢀⣾⣿⣿⣿⣿⣿⣿⣿⣶⣦⡀";
	gotoxy(x, y + OffsetY + 1);
	cout << " ⢠⣿⣿⡿⠀⠀⠈⢹⣿⣿⡿⣿⣿⣇⠀⣠⣿⣿⠟⣽⣿⣿⠇⠀⠀⢹⣿⣿⣿";
	gotoxy(x, y + OffsetY + 2);
	cout << "⠀⢸⣿⣿⡇⠀⢀⣠⣾⣿⡿⠃⢹⣿⣿⣶⣿⡿⠋⢰⣿⣿⡿⠀⠀⣠⣼⣿⣿⠏";
	gotoxy(x, y + OffsetY + 3);
	cout << "⠀⣿⣿⣿⣿⣿⣿⠿⠟⠋⠁⠀⠀⢿⣿⣿⠏⠀⠀⢸⣿⣿⣿⣿⣿⡿⠟⠋⠁";
	gotoxy(x, y + OffsetY + 4);
	cout << "⠀⠀⠀ ⠀⠀⠀⠀⠀⠀⣀⣀⣀⣸⣟⣁⣀⣀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀";
	gotoxy(x, y + OffsetY + 5);
	cout << "⣠⣴⣶⣾⣿⣿⣻⡟⣻⣿⢻⣿⡟⣛⢻⣿⡟⣛⣿⡿⣛⣛⢻⣿⣿⣶⣦⣄⡀⠀";
	gotoxy(x, y + OffsetY + 6);
	cout << "⠉⠛⠻⠿⠿⠿⠷⣼⣿⣿⣼⣿⣧⣭⣼⣿⣧⣭⣿⣿⣬⡭⠾⠿⠿⠿⠛⠉⠀";

}

void ClearLogo(int x, int y) {
	for (int i = 0; i < LogoSizeY; i++) {
		gotoxy(x, y + i + OffsetY);
		for (int j = 0; j < LogoSizeX; j++) {
			cout << " ";
		}
	}
}

void Saver() {
	RandomNumberGenerator rng;
	cout << "Для запуска анимации нажмите на любую кнопку, для завершения нажмите ESC";
	char key;
	int LogoX1 = 60; // Координаты верхнего левого угла
	int LogoY1 = 12;
	int LogoX2 = LogoX1 + LogoSizeX; // Координаты нижнего правого угла
	int LogoY2 = LogoY1 + LogoSizeY;
	int DirectionX;
	int DirectionY;
	if (rng.getRandomInRange(0, 10) < 5) DirectionX = 1; // 1 вправо, -1 влево
	else DirectionX = -1;
	if (rng.getRandomInRange(0, 10) < 5) DirectionY = 1; // 1 вправо, -1 влево
	else DirectionY = -1;

	int hits = 0;
	DrawScreen();
	DrawLogo(LogoX1, LogoY1);
	gotoxy(0, 34);
	cout << "Количество ударов об угол: " << hits;
	_getch();

	while (true) {
		if (_kbhit()) {
			key = _getch();
			if (key == 27) break;
		}
		ClearLogo(LogoX1, LogoY1);

		LogoX1 += DirectionX;
		LogoX2 += DirectionX;
		LogoY1 += DirectionY;
		LogoY2 += DirectionY;

		// Отскок от стенок по горизонтали
		if (LogoX1 <= OffsetX || LogoX2 >= ScreenSizeX - 1 + OffsetX) {
			DirectionX *= -1;
		}
		// Отскок от стенок по вертикали и проверка на удар об угол
		if (LogoY1 <= OffsetY || LogoY2 >= ScreenSizeY - 1 + OffsetY) {
			DirectionY *= -1;
			if ((LogoX1 <= OffsetX && LogoY1 <= OffsetY) || (LogoX2 >= ScreenSizeX - 1 + OffsetX && LogoY1 <= OffsetY) ||
				(LogoX1 <= OffsetX && LogoY2 >= ScreenSizeY - 1 + OffsetY) || (LogoX2 >= ScreenSizeX - 1 + OffsetX && LogoY2 >= ScreenSizeY - 1 + OffsetY)) { // wow...
				hits++;
				gotoxy(27, 34);
				cout << "  ";
				gotoxy(27, 34);
				cout << hits;
			}
		}

		DrawLogo(LogoX1, LogoY1);

		Sleep(100);
	}
}