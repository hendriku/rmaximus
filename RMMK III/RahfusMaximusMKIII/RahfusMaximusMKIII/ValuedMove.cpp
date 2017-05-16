#include "ValuedMove.h"

ValuedMove::ValuedMove(Move* moveIn, int valueIn) {
	this->x = moveIn->x;
	this->y = moveIn->y;
	value = valueIn;
}

bool ValuedMove::operator==(const ValuedMove & that) const {
	return this->value == that.value && this->x == that.x && this->y == that.y;
}

bool ValuedMove::operator<(const ValuedMove & that) const {
	return this->value < that.value;
}

bool ValuedMove::operator>(const ValuedMove & that) const {
	return this->value > that.value;
}