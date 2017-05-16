#include "InitialMoves.h"
#include <iostream>
#include <ctime>
#include <vector>

using namespace std;

static InitialMoves* _instance;

void InitialMoves::createInstance() {
	_instance = new InitialMoves();
}

InitialMoves* InitialMoves::instance() {
	return _instance;
}

InitialMoves::InitialMoves() {}

InitialMoves::~InitialMoves() {}

Move* InitialMoves::initialMove(MatchUnit* matchIn) {
	slimState = matchIn;
	currentPlayer = matchIn->getCurrentPlayer();
	Move* retMove = nullptr;
	if (slimState->getMoveNr() == 0)
		retMove = firstMove();
	else if (slimState->getMoveNr() <= 3)
		retMove = secondToFourthMove();
	else if (slimState->getMoveNr() <= 5)
		retMove = fifthOrMoreMove();
	return (retMove != nullptr && retMove->x >= 0) ? retMove : nullptr;
}

Move* InitialMoves::firstMove() {
	srand((int)time(NULL));
	int var = slimState->getXsize() / 4;
	int bx = slimState->getXsize() / 2 + rand() % var - var / 2;
	var = slimState->getYsize() / 4;
	int by = slimState->getYsize() / 2 + rand() % var - var / 2;
	if (slimState->getBoardY()->getPin(bx, by) == SWAMP)
		return randomMove();
	return new Move(bx, by);
}

Move* InitialMoves::secondToFourthMove() {
	srand((int)time(NULL));
	int bx;
	int by;
	bx = slimState->getMoveX(slimState->getMoveNr());
	by = slimState->getMoveY(slimState->getMoveNr());
	if (currentPlayer == XPLAYER) {
		if (by < slimState->getYsize() / 2)
			by = ((slimState->getYsize() - by) / 2 + by);
		else
			by /= 2;
		by = by - 1 + rand() % 3;
		if (bx <= 6) {
			bx = bx + 2 + rand() % 2;
			if (bx <= 4) {
				bx++;
			}
		}
		if (bx >= slimState->getXsize() - 6 - 1) {
			bx = bx - 2 - rand() % 2;
			if (bx >= slimState->getYsize() - 4 - 1) {
				bx--;
			}
		}
	}
	else {
		if (bx <= slimState->getXsize() / 2) {
			bx = ((slimState->getXsize() - bx) / 2 + bx);
		}
		else {
			bx /= 2;
		}
		// some random
		bx = bx - 1 + rand()%3;
		if (by <= 6) {
			by = by + 2 + rand()%2;
			if (by <= 4) {
				by++;
			}
		}
		if (by >= slimState->getYsize() - 6 - 1) {
			by = by - 2 - rand() % 2;
			if (by >= slimState->getYsize() - 4 - 1) {
				by--;
			}
		}
	}
	if (slimState->getMoveNr() == 2 || slimState->getMoveNr() == 3) {
		if (abs(bx - slimState->getMoveX(slimState->getMoveNr() - 1)) <= 2 && abs(by - slimState->getMoveY(slimState->getMoveNr() - 1)) <= 2) {
			bx = slimState->getMoveX(slimState->getMoveNr() - 1);
			by = slimState->getMoveY(slimState->getMoveNr() - 1);
			if (currentPlayer == XPLAYER) {
				bx += (bx >= slimState->getXsize() / 2) ? -5 : 5;
			}
			else {
				by += (by >= slimState->getYsize() / 2) ? -5 : 5;
			}
		}
		if (slimState->getMoveNr() == 3 && abs(bx - slimState->getMoveX(slimState->getMoveNr() - 2)) <= 2 && abs(by - slimState->getMoveY(slimState->getMoveNr() - 2)) <= 2) {
			bx = -1;
			by = -1;
		}
	}
	if (slimState->getBoardY()->getPin(bx, by) == SWAMP)
		return randomMove();
	return new Move(bx, by);
}

Move* InitialMoves::fifthOrMoreMove() {
	Move* retMove = nullptr;
	int mostFar = 0;
	int distance;
	int bx;
	int by;
	for (int i = slimState->getMoveNr(); i >= 1; i -= 2) {
		bx = slimState->getMoveX(i);
		by = slimState->getMoveY(i);
		Move* possMove = getCounterPin(bx, by);
		distance = nearestPin(possMove, i);
		if (distance > 3 && distance > mostFar) {
			mostFar = distance;
			retMove = possMove;
		} else {
			delete possMove;
		}

	}
	return retMove;
}

Move* InitialMoves::getCounterPin(int bx, int by) {
	int rx, ry;
	if (bx < 4 || by < 4)
		return nullptr;
	if (currentPlayer == XPLAYER) {
		ry = (by < slimState->getYsize() / 2) ? by + 4 : by - 4;
		rx = bx;
		if (bx < slimState->getXsize() / 2 - 3)
			rx = bx + 1;
		else if (bx > slimState->getXsize() / 2 + 3)
			rx = bx - 1;
	}
	else {
		rx = (bx < slimState->getXsize() / 2) ? bx + 4 : bx - 4;
		ry = by;
		if (by < slimState->getYsize() / 2 - 3)
			ry = by + 1;
		else if (by > slimState->getYsize() / 2 + 3)
			ry = by - 1;
	}
	if (slimState->getBoardY()->getPin(rx, ry) == SWAMP)
		return randomMove();
	return new Move(rx, ry);
}

Move* InitialMoves::randomMove() {
	cout << "T1J InitalMove ERROR - Using random move" << endl;
	vector<Move> possibleMoves = vector<Move>();
	for (int y = 0; y < slimState->getYsize(); y++)
		for (int x = 0; x < slimState->getXsize(); x++)
			if (slimState->getBoardY()->pinAllowed(x, y, slimState->getCurrentPlayer()))
				possibleMoves.push_back(Move(x, y));
	srand((int)time(NULL));
	int index = rand() % possibleMoves.size();
	Move* retMove = new Move(possibleMoves.at(index));
	return retMove;
}

int InitialMoves::nearestPin(Move * someMove, int except) {
	int dist = MAX_VALUE;
	int cDist;
	if (someMove == nullptr) {
		return -1;
	}
	for (int i = slimState->getMoveNr(); i >= 1; i--) {
		cDist = abs(slimState->getMoveX(i) - someMove->x) + abs(slimState->getMoveY(i) - someMove->y);
		if (except == slimState->getMoveNr()) {
			cDist--;
		}
		if (i != except && cDist < dist) {
			dist = cDist;
		}
	}
	return dist;
}
