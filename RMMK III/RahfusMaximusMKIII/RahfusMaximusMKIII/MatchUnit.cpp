#include "MatchUnit.h"

MatchUnit::MatchUnit() {
	realBoard = new Board();
	realBoard->zobristEnabled = true;
	altBoard = new Board();
	turn = moveNr = 0;
	currentPlayer = YPLAYER;
}

MatchUnit::MatchUnit(Board* realBoard, Board* altBoard, int turn, int currentPlayer) {
	this->realBoard = realBoard;
	realBoard->zobristEnabled = true;
	this->altBoard = altBoard;
	this->turn = this->moveNr = turn;
	this->currentPlayer = currentPlayer;
}

Board* MatchUnit::getBoard(int player) {
	return player == XPLAYER ? altBoard : realBoard;
}

Board* MatchUnit::getBoardX() {
	return altBoard;
}

Board* MatchUnit::getBoardY() {
	return realBoard;
}

vector<Move*>* MatchUnit::performMove(Move* move) {
	moveNr++;
	performedMoves.push_back(move);
	makeMove(move, currentPlayer);
	return &performedMoves;
}

vector<Move*>* MatchUnit::generateAllPossibleMoves(int player) {
	vector<Move*>* moves = new vector<Move*>();
	for (int y = 0; y < DIM; y++)
		for (int x = 0; x < DIM; x++)
			if (realBoard->pinAllowed(x, y, player))
				moves->push_back(new Move(x, y));
	return moves;
}

int MatchUnit::getTurn() {
	return turn;
}

int MatchUnit::getCurrentPlayer() {
	return currentPlayer;
}

int MatchUnit::getOtherPlayer() {
	return -currentPlayer;
}

int MatchUnit::getMoveNr() {
	return moveNr;
}

int MatchUnit::getXsize() {
	return DIM;
}

int MatchUnit::getYsize() {
	return DIM;
}

int MatchUnit::getMoveX(int nr) {
	return performedMoves.at(nr - 1)->x;
}

int MatchUnit::getMoveY(int nr) {
	return performedMoves.at(nr - 1)->y;
}

void MatchUnit::makeMove(Move* move, int player) {
	realBoard->setPin(move->x, move->y, player);
	altBoard->setPin(move->y, move->x, -player);
	nextTurn();
}

void MatchUnit::undoMove(Move* move, int player) {
	realBoard->removePin(move->x, move->y, player);
	altBoard->removePin(move->y, move->x, -player);
	preTurn();
}

void MatchUnit::switchCurrentPlayer() {
	currentPlayer *= -1;
}

void MatchUnit::nextTurn() {
	turn++;
	switchCurrentPlayer();
}

void MatchUnit::preTurn() {
	turn--;
	switchCurrentPlayer();
}
