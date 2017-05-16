#pragma once
#include <vector>
#include "Board.h"
#include "Move.h"

using namespace std;

class MatchUnit {
public:
	vector<Move*> performedMoves;

	MatchUnit();
	MatchUnit(Board* realBoard, Board* altBoard, int turn, int currentPlayer);

	Board* getBoard(int);
	Board* getBoardX();
	Board* getBoardY();
	vector<Move*>* performMove(Move*);
	vector<Move*>* generateAllPossibleMoves(int);
	int getTurn();
	int getCurrentPlayer();
	int getOtherPlayer();
	int getMoveNr();
	int getXsize();
	int getYsize();
	int getMoveX(int);
	int getMoveY(int);
	void makeMove(Move*, int);
	void undoMove(Move*, int);
	
private:
	Board* realBoard;
	Board* altBoard;
	
	int turn, moveNr, currentPlayer;
	
	void switchCurrentPlayer();
	void nextTurn();
	void preTurn();
};
