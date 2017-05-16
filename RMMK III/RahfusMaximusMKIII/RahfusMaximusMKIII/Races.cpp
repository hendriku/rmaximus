#include "Races.h"

static Races* _instance;

Races* Races::instance() {
	return _instance;
}

void Races::createInstance() {
	_instance = new Races();
}

bool Races::checkButtom(int fx, int fy, Board* boardIn, int nextPlayerIn) {
	pinX = fx;
	pinY = fy;
	board = boardIn;
	nextPlayer = nextPlayerIn;
	return blockingButtom() && checkFor5() && checkFor6() && checkFor4() && checkFor7();
}

bool Races::checkTop(int fx, int fy, Board* boardIn, int nextPlayerIn) {
	pinX = fx;
	pinY = fy;
	board = boardIn;
	nextPlayer = nextPlayerIn;
	return blockingTop() && checkFor1() && checkFor2() && checkFor0() && checkFor3();
}

// TODO Klammer checken

bool Races::checkFor0() {
	bool ret = true;
	int vx = (pinX - 1) * 2;
	int vy = (pinY - 1);
	if (board->getPin(pinX + 1, pinY + 1) == XPLAYER && vx < vy && (board->isConnected(pinX + 1, pinY + 1, pinX, pinY - 1) || (board->isConnected(pinX + 1, pinY + 1, pinX + 2, pinY + 3) && board->bridgeAllowed(pinX + 1, pinY + 1, 1) && nextPlayer == XPLAYER)))
		ret = false;
	return ret;
}

bool Races::checkFor1() {
	bool ret = true;
	int vx = (pinX - 1);
	int vy = (pinY - 1) * 2;
	if (board->getPin(pinX + 1, pinY) == XPLAYER && vx < vy && (board->isConnected(pinX + 1, pinY, pinX - 1, pinY - 1) || (board->isConnected(pinX + 1, pinY, pinX + 3, pinY + 1) && nextPlayer == XPLAYER)))
		ret = false;
	return ret;
}

bool Races::checkFor2() {
	bool ret = true;
	int vx = (board->getXsize() - 2 - pinX);
	int vy = (pinY - 1) * 2;
	if (board->getPin(pinX - 1, pinY) == XPLAYER && vx < vy && (board->isConnected(pinX - 1, pinY, pinX + 1, pinY - 1) || (board->isConnected(pinX - 1, pinY, pinX - 3, pinY + 1) && nextPlayer == XPLAYER)))
		ret = false;
	return ret;
}

bool Races::checkFor3() {
	bool ret = true;
	int vx = (board->getXsize() - 2 - pinX) * 2;
	int vy = (pinY - 1);
	if (board->getPin(pinX - 1, pinY + 1) == XPLAYER && vx < vy && (board->isConnected(pinX - 1, pinY + 1, pinX, pinY - 1) || (board->isConnected(pinX - 1, pinY + 1, pinX - 2, pinY + 3) && board->bridgeAllowed(pinX - 1, pinY + 1, 2) && nextPlayer == XPLAYER)))
		ret = false;
	return ret;
}

bool Races::checkFor4() {
	bool ret = true;
	int vx = (board->getXsize() - 2 - pinX) * 2;
	int vy = (board->getYsize() - 2 - pinY);
	if (board->getPin(pinX - 1, pinY - 1) == XPLAYER && vx < vy && (board->isConnected(pinX - 1, pinY - 1, pinX, pinY + 1) || (board->isConnected(pinX - 1, pinY - 1, pinX - 2, pinY - 3) && board->bridgeAllowed(pinX, pinY + 1, 1) && nextPlayer == XPLAYER)))
		ret = false;
	return ret;
}

bool Races::checkFor5() {
	bool ret = true;
	int vx = (board->getXsize() - 2 - pinX);
	int vy = (board->getYsize() - 2 - pinY) * 2;
	if (board->getPin(pinX - 1, pinY) == XPLAYER && vx < vy && (board->isConnected(pinX - 1, pinY, pinX + 1, pinY + 1) || (board->isConnected(pinX - 1, pinY, pinX - 3, pinY - 1) && nextPlayer == XPLAYER)))
		ret = false;
	return ret;
}

bool Races::checkFor6() {
	bool ret = true;
	int vx = (pinX - 1);
	int vy = (board->getYsize() - 2 - pinY) * 2;
	if (board->getPin(pinX + 1, pinY) == XPLAYER && vx < vy && (board->isConnected(pinX + 1, pinY, pinX - 1, pinY + 1) || (board->isConnected(pinX + 1, pinY, pinX + 3, pinY - 1) && nextPlayer == XPLAYER)))
		ret = false;
	return ret;
}

bool Races::checkFor7() {
	bool ret = true;
	int vx = (pinX - 1) * 2;
	int vy = (board->getYsize() - 2 - pinY);
	if (board->getPin(pinX + 1, pinY - 1) == XPLAYER && vx < vy && (board->isConnected(pinX + 1, pinY - 1, pinX, pinY + 1) || (board->isConnected(pinX + 1, pinY - 1, pinX + 2, pinY - 3) && board->bridgeAllowed(pinX, pinY + 1, 2) && nextPlayer == XPLAYER)))
		ret = false;
	return ret;
}

bool Races::blockingButtom() {
	return nextPlayer == YPLAYER || (checkButtomPin(pinX, pinY + 1) && checkButtomPin(pinX, pinY + 2) && checkButtomPin(pinX + 1, pinY + 1) && checkButtomPin(pinX - 1, pinY + 1) && checkButtomPinTwo(pinX, pinY + 3) && checkButtomPinTwo(pinX, pinY + 4));
}

bool Races::blockingTop() {
	return (nextPlayer == YPLAYER) || (checkTopPin(pinX, pinY - 1) && checkTopPin(pinX, pinY - 2) && checkTopPin(pinX + 1, pinY - 1) && checkTopPin(pinX - 1, pinY - 1) && checkTopPinTwo(pinX, pinY - 3) && checkTopPinTwo(pinX, pinY - 4));
}

bool Races::checkButtomPin(int oppX, int oppY) {
	return !(board->getPin(oppX, oppY) == XPLAYER && (board->isBridged(oppX, oppY, 0) || board->isBridged(oppX, oppY, 1) || board->isBridged(oppX, oppY, 2) || board->isBridged(oppX, oppY, 3) || board->isBridged(oppX + 2, oppY + 1, 0) || board->isBridged(oppX - 2, oppY + 1, 3)));
}

bool Races::checkTopPin(int oppX, int oppY) {
	return !(board->getPin(oppX, oppY) == XPLAYER && (board->isBridged(oppX, oppY, 0) || board->isBridged(oppX, oppY, 3) || board->isBridged(oppX + 2, oppY + 1, 0) || board->isBridged(oppX + 1, oppY + 2, 1) || board->isBridged(oppX - 1, oppY + 2, 2) || board->isBridged(oppX - 2, oppY + 1, 3)));
}

bool Races::checkButtomPinTwo(int oppX, int oppY) {
	return oppY >= board->getYsize() || !(board->getPin(oppX, oppY) == XPLAYER && (board->isBridged(oppX, oppY, 0) || board->isBridged(oppX, oppY, 3)));
}

bool Races::checkTopPinTwo(int oppX, int oppY) {
	return oppY <= 0 || !(board->getPin(oppX, oppY) == XPLAYER && (board->isBridged(oppX - 2, oppY + 1, 3) || board->isBridged(oppX + 2, oppY + 1, 0)));
}
