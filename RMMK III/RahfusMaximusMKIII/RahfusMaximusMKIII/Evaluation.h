#pragma once

#include <unordered_set>

class Board;
#include "Board.h"
#include "FieldData.h"
#include "CritPos.h"
#include "Pin.h"
#include "Const.h"

using namespace std;

class Evaluation {
public:
	FieldData data[MAXDIM][MAXDIM];
	int ownPinX[MAXDIM][MAXDIM];
	int rowPinCount[MAXDIM];
	int graphstX[GRAPH_SZ];
	int graphstY[GRAPH_SZ];
	
	Board* board;
	int stackcnt;

	Evaluation(Board* board);

	void evaluateY(int);
	int valueOfY(bool, int);
	void setupForY();
	void addForY(int, int);
	void removeForY(int, int, int);
	unordered_set<CritPos, CritPos::Hash>* getCritical();
private:
	unordered_set<CritPos, CritPos::Hash> critPoss;
	void evalStructure(int);
	Pin getFatherConn(int, int);
	bool calculateDistForY(int, int);
	void updateRows(int, int, int);
	void calculateRow(int, int);
	int getDistVal(int, int);
	void computeCritical(int, int);
	bool checkPlausiTop(int, int, int, int);
	bool checkPlausiBottom(int, int, int, int);
};