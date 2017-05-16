#pragma once
#include <vector>
#include <unordered_set>
#include <sstream>
#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <list>
#include "Pattern.h"
#include "Move.h"

using namespace std;

class CheckPattern {
public:
	static void createInstance();
	static CheckPattern* instance();
	
	unordered_set<Move, Move::Hash>* moves;
	list<Pattern*> offensivePatterns;
	list<Pattern*> defensivePatterns;

	void loadPattern();
	void findPatternMoves(unordered_set<Move, Move::Hash>&, bool, Board*, CritPos&, int);

private:
	void addPattern(string, bool,bool, vector<PatternElement*>&);
};
