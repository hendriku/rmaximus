#include "Move.h"
#include "Const.h"

using namespace std;

Move::Move(int x, int y, int player) {
	if (player == XPLAYER) {
		this->x = y;
		this->y = x;
	} else {
		this->x = x;
		this->y = y;
	}
}

Move::Move(const Move * that) {
	this->x = that->x;
	this->y = that->y;
}

// Wird für Vererbung von ValuedMove benötigt
Move::Move(){}

bool Move::operator==(const Move& m) const {
	return m.x == x && m.y == y;
}

bool Move::operator==(const Move* that) const {
	return that->x == this->x && that->y == this->y;
}

size_t Move::Hash::operator()(const Move & m) const {
	return (m.x << SHIFT) + m.y;
}
