#pragma once
#include "Move.h"
#include "MatchUnit.h"
#include "ValuedMove.h"
#include <unordered_set>
#include <vector>
#include <iterator>

class OrderedMoves
{
public:
	static void staticInit(MatchUnit*);
	static void initOrderedMoves();
	static void addValuedMove(Move&, int);
	static void addKiller(Move&, int);
	static void generateMoves(vector <ValuedMove>&, int, bool);
	static int getSortValue(const Move&, int);
	static bool isGameOver();
private:
	static Move* randomMove(int);
	static void generateNewMoves(vector <ValuedMove>&, int);
	static void sortMoves(vector <ValuedMove>&, unordered_set<Move, Move::Hash>&, int);
};