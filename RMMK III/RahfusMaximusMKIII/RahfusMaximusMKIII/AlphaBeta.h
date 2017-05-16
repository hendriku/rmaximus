#include "MatchUnit.h"
#include <map>

class AlphaBeta {
public:
	static void createInstance();
	static AlphaBeta* instance();

	Move* calculate(MatchUnit*);
private:
	map<int, int> zobristMap;
	MatchUnit* slimState;
	Move* iterationMove;
	int bestEval;
	int evaluations;
	int zobristLookups;
	int alphaBetaCuts;
	int iterationDepth;
	int currentPlayer;
	bool usealphabeta;

	void info();
	void handleLogicFields(MatchUnit* m);
	void setIterationMove(Move&);
	int alphaBeta(int, int, int, int);
	int evaluatePosition(int);
};
