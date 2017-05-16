#pragma once
#include "MatchUnit.h"
#include "Move.h"
#include "Const.h"

class InitialMoves
{
public:
	static void createInstance();
	static InitialMoves* instance();

	Move* initialMove(MatchUnit*);

private:
	MatchUnit* slimState;
	int currentPlayer;

	InitialMoves();
	~InitialMoves();

	Move* firstMove();
	Move* secondToFourthMove();
	Move* fifthOrMoreMove();
	Move* getCounterPin(int, int);
	Move* randomMove();
	int nearestPin(Move*, int);
};