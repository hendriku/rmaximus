#include "CritPos.h"
#include "Move.h"

CritPos::CritPos(int xin, int yin, bool dirin) {
	x = xin;
	y = yin;
	dir = dirin;
}

bool CritPos::isDir() {
	return dir;
}

int CritPos::getX() {
	return x;
}

int CritPos::getY() {
	return y;
}

bool CritPos::operator==(const CritPos & that) const {
	return this->x == that.x && this->y == that.y && this->dir == that.dir;
}

bool CritPos::operator==(const CritPos * that) const {
	return  this->x == that->x && this->y == that->y && this->dir == that->dir;
}

size_t CritPos::Hash::operator()(const CritPos & m) const {
	return (((m.x << 5) + m.y) << 1) + m.dir;
}
