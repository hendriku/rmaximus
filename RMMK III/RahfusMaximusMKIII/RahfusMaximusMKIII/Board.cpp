#include <iostream>
#include "Board.h"
#include "Zobrist.h"

using namespace std;

Board::Board() {
	initEval();
	initZobrist();
}

bool Board::setPin(int xin, int yin, int player) {
	if (!pinAllowed(xin, yin, player))
		return false;

	if (zobristEnabled)
		zobristValue ^= Zobrist::instance()->getPinValue(xin, yin, player);

	int x = xin + MARGIN;
	int y = yin + MARGIN;

	fields[x][y].value = player;
	if (fields[x - 2][y - 1].value == player)
		setBridge(x, y, 0);
	if (fields[x - 1][y - 2].value == player)
		setBridge(x, y, 1);
	if (fields[x + 1][y - 2].value == player)
		setBridge(x, y, 2);
	if (fields[x + 2][y - 1].value == player)
		setBridge(x, y, 3);
	if (fields[x + 2][y + 1].value == player)
		setBridge(x + 2, y + 1, 0);
	if (fields[x + 1][y + 2].value == player)
		setBridge(x + 1, y + 2, 1);
	if (fields[x - 1][y + 2].value == player)
		setBridge(x - 1, y + 2, 2);
	if (fields[x - 2][y + 1].value == player)
		setBridge(x - 2, y + 1, 3);

	_eval->addForY(xin, yin);
	return true;
}

bool Board::removePin(int xin, int yin, int player) {
	int x = xin + MARGIN;
	int y = yin + MARGIN;

	if (fields[x][y].value == 0)
		return false;

	if (zobristEnabled)
		zobristValue ^= Zobrist::instance()->getPinValue(xin, yin, player);

	fields[x][y].value = 0;
	removeBridge(x, y, 0);
	removeBridge(x, y, 1);
	removeBridge(x, y, 2);
	removeBridge(x, y, 3);
	removeBridge(x + 2, y + 1, 0);
	removeBridge(x + 1, y + 2, 1);
	removeBridge(x - 1, y + 2, 2);
	removeBridge(x - 2, y + 1, 3);
	_eval->removeForY(xin, yin, player);
	return true;
}

bool Board::pinAllowed(int xin, int yin, int player) {
	return !((player == XPLAYER && (yin < 1 || yin > getYsize() - 2 || xin < 0 || xin > getXsize() - 1)) || (player == YPLAYER && (yin < 0 || yin > getYsize() - 1 || xin < 1 || xin > getXsize() - 2)) || (fields[xin + 3][yin + 3].value != 0));
}

bool Board::isConnected(int xa, int ya, int xb, int yb) {
	if (abs(xa - xb) >= 3 || abs(ya - yb) >= 3 || abs(xa - xb) + abs(ya - yb) != 3)
		cout << "INVALID DIRECTION IsConnected" << endl;
	if (ya < yb)
		return isBridged(xb, yb, (xa < xb) ? xa - xb + 2 : xa - xb + 1);
	else
		return isBridged(xa, ya, (xb < xa) ? xb - xa + 2 : xb - xa + 1);
}

bool Board::isBridgeAllowed(int xa, int ya, int xb, int yb) {
	if (abs(xa - xb) >= 3 || abs(ya - yb) >= 3 || abs(xa - xb) + abs(ya - yb) != 3)
		cout << "INVALID DIRECTION IsBridgeAllowed" << endl;
	if (ya < yb)
		return bridgeAllowed(xb, yb, (xa < xb) ? xa - xb + 2 : xa - xb + 1);
	else
		return bridgeAllowed(xa, ya, (xb < xa) ? xb - xa + 2 : xb - xa + 1);
}

int Board::getPin(int xin, int yin) {
	return fields[xin + MARGIN][yin + MARGIN].value;
}

bool Board::isBridged(int x, int y, int direction) {
	return fields[x + MARGIN][y + MARGIN].bridge[direction] >= BRIDGED;
}

bool Board::bridgeAllowed(int x, int y, int direction) {
	return fields[x + MARGIN][y + MARGIN].bridge[direction] == 0;
}

bool Board::isStrong(int hx, int hy) {
	return isBridged(hx, hy, 0) || isBridged(hx, hy, 1) || isBridged(hx, hy, 2) || isBridged(hx, hy, 3) || isBridged(hx + 2, hy + 1, 0) || isBridged(hx + 1, hy + 2, 1) || isBridged(hx - 1, hy + 2, 2) || isBridged(hx - 2, hy + 1, 3);
}

Evaluation* Board::getEval() {
	return _eval;
}

bool Board::checkGameOver() {
	_eval->evaluateY(XPLAYER);
	return (_eval->valueOfY(false, XPLAYER) == 0);
}

int Board::getZobristValue() {
	return zobristValue;
}

int Board::getXsize() {
	return DIM;
}

int Board::getYsize() {
	return DIM;
}

void Board::initEval() {
	_eval = new Evaluation(this);
	_eval->setupForY();
}

void Board::initZobrist() {
	zobristEnabled = false;
	zobristValue = 0;
}

bool Board::setBridge(int x, int y, int direction) {
	if (fields[x][y].bridge[direction] > 0) 
		return true;
	fields[x][y].bridge[direction] = BRIDGED;
	zobristValue ^= Zobrist::instance()->getLinkValue(x, y, direction);
	switch (direction) {
	case 0:
		fields[x][y + 1].bridge[1]++;
		fields[x - 1][y + 1].bridge[2]++;
		fields[x - 2][y + 1].bridge[2]++;
		fields[x - 1][y].bridge[3]++;
		fields[x - 1][y].bridge[2]++;
		fields[x - 1][y].bridge[1]++;
		fields[x - 2][y].bridge[3]++;
		fields[x - 2][y].bridge[2]++;
		fields[x - 3][y].bridge[3]++;
		break;
	case 1:
		fields[x - 1][y + 1].bridge[2]++;
		fields[x + 1][y].bridge[0]++;
		fields[x - 1][y].bridge[3]++;
		fields[x - 1][y].bridge[2]++;
		fields[x - 2][y].bridge[3]++;
		fields[x][y - 1].bridge[0]++;
		fields[x - 1][y - 1].bridge[3]++;
		fields[x - 1][y - 1].bridge[2]++;
		fields[x - 2][y - 1].bridge[3]++;
		break;
	case 2:
		fields[x + 1][y + 1].bridge[1]++;
		fields[x - 1][y].bridge[3]++;
		fields[x + 1][y].bridge[0]++;
		fields[x + 1][y].bridge[1]++;
		fields[x + 2][y].bridge[0]++;
		fields[x][y - 1].bridge[3]++;
		fields[x + 1][y - 1].bridge[0]++;
		fields[x + 1][y - 1].bridge[1]++;
		fields[x + 2][y - 1].bridge[0]++;
		break;
	case 3:
		fields[x][y + 1].bridge[2]++;
		fields[x + 1][y + 1].bridge[1]++;
		fields[x + 2][y + 1].bridge[1]++;
		fields[x + 1][y].bridge[0]++;
		fields[x + 1][y].bridge[1]++;
		fields[x + 1][y].bridge[2]++;
		fields[x + 2][y].bridge[0]++;
		fields[x + 2][y].bridge[1]++;
		fields[x + 3][y].bridge[0]++;
		break;
	}
	return true;
}

void Board::removeBridge(int x, int y, int direction) {
	if (fields[x][y].bridge[direction] < BRIDGED)
		return;
	fields[x][y].bridge[direction] = 0;
	zobristValue ^= Zobrist::instance()->getLinkValue(x, y, direction);
	switch (direction) {
	case 0:
		fields[x][y + 1].bridge[1]--;
		fields[x - 1][y + 1].bridge[2]--;
		fields[x - 2][y + 1].bridge[2]--;
		fields[x - 1][y].bridge[3]--;
		fields[x - 1][y].bridge[2]--;
		fields[x - 1][y].bridge[1]--;
		fields[x - 2][y].bridge[3]--;
		fields[x - 2][y].bridge[2]--;
		fields[x - 3][y].bridge[3]--;
		break;
	case 1:
		fields[x - 1][y + 1].bridge[2]--;
		fields[x + 1][y].bridge[0]--;
		fields[x - 1][y].bridge[3]--;
		fields[x - 1][y].bridge[2]--;
		fields[x - 2][y].bridge[3]--;
		fields[x][y - 1].bridge[0]--;
		fields[x - 1][y - 1].bridge[3]--;
		fields[x - 1][y - 1].bridge[2]--;
		fields[x - 2][y - 1].bridge[3]--;
		break;
	case 2:
		fields[x + 1][y + 1].bridge[1]--;
		fields[x - 1][y].bridge[3]--;
		fields[x + 1][y].bridge[0]--;
		fields[x + 1][y].bridge[1]--;
		fields[x + 2][y].bridge[0]--;
		fields[x][y - 1].bridge[3]--;
		fields[x + 1][y - 1].bridge[0]--;
		fields[x + 1][y - 1].bridge[1]--;
		fields[x + 2][y - 1].bridge[0]--;
		break;
	case 3:
		fields[x][y + 1].bridge[2]--;
		fields[x + 1][y + 1].bridge[1]--;
		fields[x + 2][y + 1].bridge[1]--;
		fields[x + 1][y].bridge[0]--;
		fields[x + 1][y].bridge[1]--;
		fields[x + 1][y].bridge[2]--;
		fields[x + 2][y].bridge[0]--;
		fields[x + 2][y].bridge[1]--;
		fields[x + 3][y].bridge[0]--;
		break;
	}
}