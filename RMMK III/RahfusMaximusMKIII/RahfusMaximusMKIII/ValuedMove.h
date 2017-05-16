#pragma once
#include "Move.h"
struct ValuedMove : public Move {
	int value;
	
	ValuedMove(Move* moveIn, int valueIn);

	bool operator==(const ValuedMove&) const;
	bool operator<(const ValuedMove&) const;
	bool operator>(const ValuedMove&) const;
};

