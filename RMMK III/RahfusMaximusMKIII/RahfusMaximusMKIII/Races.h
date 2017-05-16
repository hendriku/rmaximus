#pragma once
#include "Board.h"

class Races {
public:
	static Races* instance();
	static void createInstance();

	bool checkButtom(int, int, Board*, int);
	bool checkTop(int, int, Board*, int);
private:
	Board* board;
	int pinX, pinY, nextPlayer;

	bool checkFor0();
	bool checkFor1();
	bool checkFor2();
	bool checkFor3();
	bool checkFor4();
	bool checkFor5();
	bool checkFor6();
	bool checkFor7();

	bool blockingButtom();
	bool blockingTop();

	bool checkButtomPin(int, int);
	bool checkTopPin(int, int);
	bool checkButtomPinTwo(int, int);
	bool checkTopPinTwo(int, int);
};