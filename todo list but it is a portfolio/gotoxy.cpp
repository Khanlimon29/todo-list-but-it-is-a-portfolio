#include "CLI_QOL.h"
#include <windows.h>
#include <iostream>

void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void SetColor(int color) {
    std::cout << "\033[1;" << color << "m";
}