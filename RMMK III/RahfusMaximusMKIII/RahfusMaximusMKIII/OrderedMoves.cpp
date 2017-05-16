#include "OrderedMoves.h"
#include "ValuedMove.h"
#include "CheckPattern.h"
#include <unordered_map>
#include <ctime>
#include <list>

using namespace std;

static MatchUnit* slimState;

static list<ValuedMove>* valuedMoves;
static unordered_map <Move, int, Move::Hash>* killerMoves0;
static unordered_map <Move, int, Move::Hash>* killerMoves1;

static bool gameover;

void OrderedMoves::staticInit(MatchUnit* mup) {
	slimState = mup;
	valuedMoves = new list<ValuedMove>();
	killerMoves0 = new unordered_map<Move, int, Move::Hash>();
	killerMoves1 = new unordered_map<Move, int, Move::Hash>();
}

void OrderedMoves::initOrderedMoves() {
	valuedMoves->clear();
	killerMoves0->clear();
	killerMoves1->clear();
}

void OrderedMoves::addValuedMove(Move& move, int value) {
	valuedMoves->push_back(ValuedMove(&move, value));
}

void OrderedMoves::addKiller(Move& move, int player) {
	if (player == XPLAYER) {
		auto iterator = killerMoves0->find(move);
		if (iterator == killerMoves0->end())
			killerMoves0->insert(pair<Move, int>(Move(move), 1));
		else
			iterator->second++;
	}
	else {
		auto iterator = killerMoves1->find(move);
		if (iterator == killerMoves1->end())
			killerMoves1->insert(pair<Move, int>(Move(move), 1));
		else
			iterator->second++;
	}
}

bool OrderedMoves::isGameOver() {
	return gameover;
}

void OrderedMoves::generateMoves(vector<ValuedMove>& moves, int player, bool isMaxPly) {
	if (isMaxPly && valuedMoves->size() > 0) {
		if (player == YPLAYER)
			valuedMoves->sort([](const ValuedMove& a, const ValuedMove& b) { return a > b; });
		else
			valuedMoves->sort([](const ValuedMove& a, const ValuedMove& b) { return a < b; });

		for (const ValuedMove& vmp : *valuedMoves)
			moves.push_back(vmp);

		valuedMoves->clear();
	}
	else {
		generateNewMoves(moves, player);
	}
}

void OrderedMoves::generateNewMoves(vector<ValuedMove>& container, int player) {
	gameover = false;

	unordered_set<Move, Move::Hash> moves;

	// Caching
	Board* ownSlimBoard = slimState->getBoard(player);
	Evaluation* ownEval = ownSlimBoard->getEval();
	Board* oppSlimBoard = slimState->getBoard(-player);
	Evaluation* oppEval = oppSlimBoard->getEval();

	oppEval->evaluateY(YPLAYER);
	int oppVal = oppEval->valueOfY(true, YPLAYER);
	if (oppVal == 0) {
		gameover = true;
		return; // Check if okay
	}
	ownEval->evaluateY(YPLAYER);
	ownEval->valueOfY(true, YPLAYER);

	unordered_set<CritPos, CritPos::Hash>* criticals = ownEval->getCritical();

	if (criticals->empty()) {
		int xc = slimState->getMoveX(slimState->getMoveNr());
		int yc = slimState->getMoveY(slimState->getMoveNr());
		criticals->insert(CritPos(xc, yc, DOWN));
		criticals->insert(CritPos(xc, yc, UP));
	}

	for (CritPos element : *criticals) {
		int xe = element.getX();
		int ye = element.getY();
		if (!element.isDir()) {
			if (ownSlimBoard->bridgeAllowed(xe, ye, 1) && ownSlimBoard->pinAllowed(xe - 1, ye - 2, YPLAYER))
				moves.insert(Move(xe - 1, ye - 2, player));
			else if (ownSlimBoard->bridgeAllowed(xe, ye, 0) && ownSlimBoard->pinAllowed(xe - 2, ye - 1, YPLAYER))
				moves.insert(Move(xe - 2, ye - 1, player));
			if (ownSlimBoard->bridgeAllowed(xe, ye, 2) && ownSlimBoard->pinAllowed(xe + 1, ye - 2, YPLAYER))
				moves.insert(Move(xe + 1, ye - 2, player));
			else if (ownSlimBoard->bridgeAllowed(xe, ye, 3) && ownSlimBoard->pinAllowed(xe + 2, ye - 1, YPLAYER))
				moves.insert(Move(xe + 2, ye - 1, player));
		}
		else {
			if (ownSlimBoard->bridgeAllowed(xe - 1, ye + 2, 2) && ownSlimBoard->pinAllowed(xe - 1, ye + 2, YPLAYER))
				moves.insert(Move(xe - 1, ye + 2, player));
			else if (ownSlimBoard->bridgeAllowed(xe - 2, ye + 1, 3) && ownSlimBoard->pinAllowed(xe - 2, ye + 1, YPLAYER))
				moves.insert(Move(xe - 2, ye + 1, player));
			if (ownSlimBoard->bridgeAllowed(xe + 1, ye + 2, 1) && ownSlimBoard->pinAllowed(xe + 1, ye + 2, YPLAYER))
				moves.insert(Move(xe + 1, ye + 2, player));
			else if (ownSlimBoard->bridgeAllowed(xe + 2, ye + 1, 0) && ownSlimBoard->pinAllowed(xe + 2, ye + 1, YPLAYER))
				moves.insert(Move(xe + 2, ye + 1, player));
		}
		CheckPattern::instance()->findPatternMoves(moves, true, ownSlimBoard, element, player);
	}

	criticals = oppEval->getCritical();

	for (CritPos element : *criticals) {
		int ye = element.getX();
		int xe = element.getY();
		xe += (element.isDir() ? 4 : -4);
		ye += ((ye > slimState->getYsize() / 2 + 3) ? -1 : (ye < slimState->getYsize() / 2 - 3) ? 1 : 0);

		if (ownSlimBoard->pinAllowed(xe, ye, YPLAYER))
			moves.insert(Move(xe, ye, player));

		CheckPattern::instance()->findPatternMoves(moves, false, oppSlimBoard, element, player);
	}

	if (moves.empty())
		moves.insert(randomMove(player));
	sortMoves(container, moves, player);
}

int OrderedMoves::getSortValue(const Move& move, int ref) {
	if (ref == 0) {
		auto iterator = killerMoves0->find(move);
		return (iterator == killerMoves0->end()) ? 0 : iterator->second;
	}
	else {
		auto iterator = killerMoves1->find(move);
		return (iterator == killerMoves1->end()) ? 0 : iterator->second;
	}
}

Move* OrderedMoves::randomMove(int player) {
	int bx, by;
	srand((int)
		(NULL));
	do {
		bx = rand() % slimState->getXsize();
		by = rand() % slimState->getYsize();
	} while (!slimState->getBoard(YPLAYER)->pinAllowed(bx, by, player));
	return new Move(bx, by);
}

void OrderedMoves::sortMoves(vector<ValuedMove>& container, unordered_set<Move, Move::Hash>& moves, int player) {
	int ref = (player == XPLAYER) ? 0 : 1;

	for (Move m : moves)
		container.push_back(ValuedMove(&m, getSortValue(m, ref)));

	// TODO Check ob es richtig rum sortiert wird & ein guter Algorithmus benutzt wird.
	std::sort(container.begin(), container.end(), [](const ValuedMove& a, const ValuedMove& b) { return a > b; });
}
