#pragma once

#include "Const.h"
#include "Field.h"

class Evaluation;

#include "Evaluation.h"

class Board {
public:
	Field fields[ARRAYDIM][ARRAYDIM];
	bool zobristEnabled;

	Board();

	bool setPin(int, int, int);
	bool removePin(int, int, int);
	bool pinAllowed(int, int, int);
	bool isConnected(int, int, int, int);
	bool isBridgeAllowed(int, int, int, int);
	int getPin(int, int);
	bool isBridged(int, int, int);
	bool bridgeAllowed(int, int, int);
	bool isStrong(int, int);
	Evaluation* getEval();
	bool checkGameOver();
	int getZobristValue();
	int getXsize();
	int getYsize();

private:
	Evaluation* _eval;
	int zobristValue;

	void initEval();
	void initZobrist();
	bool setBridge(int, int, int);
	void removeBridge(int, int, int);
	
};
