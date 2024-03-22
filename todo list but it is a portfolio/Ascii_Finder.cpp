#include <iostream>
#include <conio.h>
#include "Ascii_Finder.h"

using namespace std;

void ASCIIfinder() {
    cout << "Нажмите на кнопку ";
    int show = _getch();
    cout << show;
    cout << "\nНажмите на любую кнопку для выхода";
    _getch();
}