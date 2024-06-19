#include "tic_tac_toe.h"
#include "gotoxy.h"
#include <iostream>
#include <conio.h>
#include <vector>

using namespace std;


bool WinCheck(string game[3][3]) {
    // Строки
    for (int i = 0; i < 3; i++) {
        if (game[i][0] == game[i][1] && game[i][1] == game[i][2] && game[i][0] != "") {
            return true;
        }
    }

    // Столбцы
    for (int i = 0; i < 3; i++) {
        if (game[0][i] == game[1][i] && game[1][i] == game[2][i] && game[0][i] != "") {
            return true;
        }
    }

    // Диагонали
    if (game[0][0] == game[1][1] && game[1][1] == game[2][2] && game[0][0] != "") {
        return true;
    }
    if (game[0][2] == game[1][1] && game[1][1] == game[2][0] && game[0][2] != "") {
        return true;
    }
    return false;
}

void tic_tac() {
    string game[3][3];
    int numbOfOptX = 12;
    int numbOfOptY = 6;
    int currOptX = 6;
    int currOptY = 2;
    int step = 0;
    char keyT;
    bool running = true;
    bool cross = true;
    cout << "↑↓→← - управление по полю, Enter - установка знака (X первый)\n\n";
    cout << "    |   |   \n -----------\n    |   |   \n -----------\n    |   |   ";
    gotoxy(currOptX, currOptY + 4);
    while (running) {
        if (_kbhit()) {
            keyT = _getch();
            switch (keyT) {
            case 72: // Up
            {
                currOptY = (currOptY - 2 + numbOfOptY) % numbOfOptY;
                gotoxy(currOptX, currOptY + 4);
                break;
            }
            case 80: // Down
            {
                currOptY = (currOptY + 2 + numbOfOptY) % numbOfOptY;
                gotoxy(currOptX, currOptY + 4);
                break;
            }
            case 77: // right 
            {
                currOptX = (currOptX + 4 + numbOfOptX) % numbOfOptX;
                gotoxy(currOptX, currOptY + 4);
                break;
            }
            case 75: // left
            {
                currOptX = (currOptX - 4 + numbOfOptX) % numbOfOptX;
                gotoxy(currOptX, currOptY + 4);
                break;
            }

            case 13: // Enter
            {

                if (cross == true) {
                    if (game[currOptX / 4][currOptY / 2] != "O") {
                        game[currOptX / 4][currOptY / 2] = "X";
                        cout << "X";
                        cross = false;
                        step++;
                    }
                }
                else {
                    if (game[currOptX / 4][currOptY / 2] != "X") {
                        game[currOptX / 4][currOptY / 2] = "O";
                        cout << "O";
                        cross = true;
                        step++;
                    }
                }
                
                if (step >= 5) {
                    if (WinCheck(game)) {
                        running = false;
                        gotoxy(0,10);
                        cout << "Победил ";
                        if (!cross) cout << "первый игрок (X)";
                        if (cross) cout << "второй игрок (O)";
                        cout << "\nНажмите на любую кнопку для выхода";
                    }
                    else if (step == 9 && running == true)
                    {
                        running = false;
                        gotoxy(0, 10);
                        cout << "Ничья\nНажмите на любую кнопку для выхода";
                    }
                }
                gotoxy(currOptX, currOptY + 4);
                break;
            }

            case 27: // Esc
            {
                running = false;
                gotoxy(0, 9);
                cout << "Игра заверешена\nНажмите на любую кнопку для выхода";
                break;

            }
            }
        }

    }
    _getch();
    return;
}