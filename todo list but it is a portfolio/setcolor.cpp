#include "setcolor.h"
#include <iostream>

void SetColor(int color) {
	std::cout << "\033[1;" << color << "m";
}
