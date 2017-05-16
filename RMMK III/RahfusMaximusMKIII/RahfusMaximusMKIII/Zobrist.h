#pragma once
#include <unordered_set>
#include "Const.h"

using namespace std;

class Zobrist {
public:
	static Zobrist* instance();
	static void createInstance();

	int getPinValue(int, int, int);
	int getLinkValue(int, int, int);
	void initialize();
private:
	struct ZobristField {
		int bridge[4];
		int valueX, valueY;
	};

	ZobristField fields[MAXDIM][MAXDIM];
	unordered_set<int> alreadyDrawn;
	
};