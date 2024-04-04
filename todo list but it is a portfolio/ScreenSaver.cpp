#include "ScreenSaver.h"
#include <iostream>
#include <conio.h>
#include "gotoxy.h"

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

void ClearScreen() {
	gotoxy(1, 5);
	for (int i =  4; i < ScreenSizeY + 4; i++) {
		for (int j = 1; j < ScreenSizeX; j++) {
			cout << " ";
		}
		gotoxy(1, i);
	}
}


void Saver() {
	cout << "Для запуска анимации нажмите на любую кнопку, для завершения нажмите ESC";
	char key;
	int LogoX1 = 60; // координаты верхнего левого угла
	int LogoY1 = 12;
	int LogoX2 = 60 + LogoSizeX; // координаты нижнего правого угла
	int LogoY2 = 12 + LogoSizeY;
	int DirectionX = 1; // Направление по оси X: 1 вправо, -1 влево
	int DirectionY = 1; // Направление по оси Y: 1 вниз, -1 вверх
	DrawScreen();
	DrawLogo(LogoX1, LogoY1);
	_getch();

	while (true) {
		if (_kbhit()) {
			key = _getch();
			if (key == 27) break;
		}
		ClearScreen();
		// Перемещение логотипа
		LogoX1 += DirectionX;
		LogoX2 += DirectionX;
		LogoY1 += DirectionY;
		LogoY2 += DirectionY;

		// Отскок от стенок по горизонтали
		if (LogoX1 <= OffsetX || LogoX2 >= ScreenSizeX - 1 + OffsetX) {
			DirectionX *= -1;
		}
		// Отскок от стенок по вертикали
		if (LogoY1 <= OffsetY || LogoY2 >= ScreenSizeY - 1 + OffsetY) {
			DirectionY *= -1;
		}

		// Отображение нового положения логотипа
		DrawLogo(LogoX1, LogoY1);
		Sleep(5);
	}
}