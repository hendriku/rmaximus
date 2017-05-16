#include "Pin.h"

Pin::Pin(int x, int y) {
	this->x = x;
	this->y = y;
}

int Pin::getX() {
	return x;
}

int Pin::getY() {
	return y;
}
