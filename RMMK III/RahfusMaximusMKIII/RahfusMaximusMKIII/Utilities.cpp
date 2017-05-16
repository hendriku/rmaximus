#include <string>
#include <vector>
#include "Utilities.h"

using namespace std;

static HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

int Utilities::indexOf(string src, string key) {
	return (int)src.find(key, 0);
}

WORD Utilities::getPinColor(int x, int y, MatchUnit* m) {
	if ((x == 0 || x == 23) && (y == 0 || y == 23))
		return 800;
	if (x == 0 || x == 23)
		return 144;
	if (y == 0 || y == 23)
		return 69;
	switch (m->getBoardY()->getPin(x, y)) {
	case 0:
		return 132;
	case -1:
		return 453;
	case 1:
		return 19;
	case -2:
		return 800;
	default:
		return 255;
	}
}

void Utilities::printBoard(MatchUnit* m) {
	cout << endl << "\t\t"; // start 3 down, 3 tabs, right
	for (int y = 0; y < DIM; y++) {
		for (int x = 0; x < DIM; x++) {
			SetConsoleTextAttribute(console, getPinColor(x,y,m)); // set color for the next print
			cout << "  ";
		}
		SetConsoleTextAttribute(console, 0); // set color to black background
		cout << endl << "\t\t"; // Start next line, 1 down, 3 tabs over
	}
	SetConsoleTextAttribute(console, 15); // set color to black background, white chars
}