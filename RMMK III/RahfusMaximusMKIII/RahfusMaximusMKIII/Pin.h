#pragma once

struct Pin {
	Pin(int, int);

	int getX();
	int getY();
private:
	int x, y;
};