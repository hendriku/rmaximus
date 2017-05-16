#pragma once
#include "Const.h"
#include <string>

using namespace std;

struct Move {
	int x, y;

	Move(int x, int y, int player = YPLAYER);
	Move(const Move*);
	Move();

	bool operator==(const Move& m) const;

	bool operator==(const Move* that) const;
	
	struct Hash {
		size_t operator()(const Move& m) const;
	};
};