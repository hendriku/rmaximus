#include "AlphaBeta.h"
#include "InitialMoves.h"
#include "LinuxTimeUtils.h"
#include "Races.h"
#include "Zobrist.h"
#include "PatternElement.h"
#include "CheckPattern.h"
#include "OrderedMoves.h"
#include <iostream>
#include <map>

using namespace std;

static AlphaBeta* _instance;

void AlphaBeta::createInstance() {
	_instance = new AlphaBeta();
}

AlphaBeta* AlphaBeta::instance() {
	return _instance;
}

bool timeIsUp() {
	return LinuxTimeUtils::GetDeltaTime() >= MAX_CALCULATION_TIME;
}

Move* AlphaBeta::calculate(MatchUnit* m) {
	handleLogicFields(m);

	// Versuchen, einen Anfangszug zu ermitteln
	iterationMove = InitialMoves::instance()->initialMove(m);
	if (iterationMove == nullptr) {
		bool nextIteration = true;
		for (iterationDepth = BASE_DEPTH; nextIteration; iterationDepth++) {
			// Die 4. Iteration ist nach T1J unnötig, da man in den meisten Fällen nach der 3. auch noch die 4te schafft.
			if (iterationDepth != 4) {
				zobristMap.clear();
				bestEval = alphaBeta(currentPlayer, iterationDepth, MIN_VALUE, MAX_VALUE);
				usealphabeta = true;
				// Abbruchbedingung IDDFS: Entweder Zeit abgelaufen oder bereits bis zum Ende gerechnet
				nextIteration = !(timeIsUp() || (slimState->getTurn() + iterationDepth) >= GAME_ENDING_TURN);
				if(!nextIteration) 
					cout << "BROKEN: ";
				info();
			}
		}
	}
	else {
		info();
	}
	if (iterationMove == nullptr)
		cout << "Fail! No move was found. :(" << endl;
	return iterationMove;
}

void AlphaBeta::info() {
	cout << "[" << slimState->getTurn() << "]" << "+" << iterationDepth << " T: " << LinuxTimeUtils::GetDeltaTime() << ", E: " << evaluations << ", ZL: " << zobristLookups << ", BeE: " << bestEval << ", ABC: " << alphaBetaCuts << endl;
}

void AlphaBeta::handleLogicFields(MatchUnit* m) {
	this->slimState = m;
	this->currentPlayer = m->getCurrentPlayer();
	usealphabeta = false;
	iterationMove = nullptr;
	bestEval = alphaBetaCuts = evaluations = zobristLookups = 0;
	OrderedMoves::initOrderedMoves();
}

void AlphaBeta::setIterationMove(Move& m) {
	delete iterationMove;
	iterationMove = new Move(m);
}

int AlphaBeta::alphaBeta(int player, int depth, int alpha, int beta) {
	if (timeIsUp()) {
		// handleSendMove();
		return 0;
	}

	int val;
	if (depth == 0) {
		auto iterator = zobristMap.find(slimState->getBoardY()->getZobristValue());
		if (iterator != zobristMap.end()) {
			zobristLookups++;
			return iterator->second;
		}
		evaluations++;
		int val = evaluatePosition(player);
		zobristMap.insert(pair<int, int>(slimState->getBoardY()->getZobristValue(), val));
		return val;
	}

	OrderedMoves moveManager = OrderedMoves();
	vector<ValuedMove> pms;
	moveManager.generateMoves(pms, player, depth == iterationDepth);

	// TODO Wenn turn == 60 berechne wer die meisten Punkte hat und berwerte absolut
	// Codevorschlag:
	// if(slimState->getTurn() == 60) return (GAMEOVER + depth) * PointsCalculator::GetWinningPlayer(slimState);
	if (moveManager.isGameOver()) return (GAMEOVER + depth) * player;

	if (player == XPLAYER) {
		for (Move move : pms) {
			slimState->makeMove(&move, player);
			val = alphaBeta(-player, depth - 1, alpha, beta);
			if (depth == iterationDepth)
				OrderedMoves::addValuedMove(move, val);
			if (val < beta) {
				if (depth == iterationDepth) {
					if (!timeIsUp())
						setIterationMove(move);
					if (usealphabeta)
						beta = val;
				}
				else {
					beta = val;
					OrderedMoves::addKiller(move, player);
				}
			}
			// TODO Testen, ob kopieren schneller ist
			slimState->undoMove(&move, player);
			if (beta <= alpha) {
				alphaBetaCuts++;
				return alpha;
			}
			// Kein AlphaBetaCut, aber Zeit vorbei
			if (timeIsUp()) {
				//handleSendMove();
				return 0;
			}
		}
		return beta;
	}
	else {
		for (Move move : pms) {
			slimState->makeMove(&move, player);
			val = alphaBeta(-player, depth - 1, alpha, beta);
			if (depth == iterationDepth)
				OrderedMoves::addValuedMove(move, val);
			if (val > alpha) {
				if (depth == iterationDepth) {
					if (!timeIsUp())
						setIterationMove(move);
					if (usealphabeta) {
						alpha = val;
					}
				}
				else {
					alpha = val;
					OrderedMoves::addKiller(move, player);
				}
			}
			// TODO Testen, ob kopieren schneller ist
			slimState->undoMove(&move, player);
			if (beta <= alpha) {
				alphaBetaCuts++;
				return beta;
			}
			// Kein AlphaBetaCut, aber Zeit vorbei
			if (timeIsUp()) {
				//handleSendMove();
				return 0;
			}
		}
		return alpha;
	}
}


int AlphaBeta::evaluatePosition(int player) {
	int val1, val2;
	slimState->getBoardY()->getEval()->evaluateY(player);
	val1 = slimState->getBoardY()->getEval()->valueOfY(false, player);
	slimState->getBoardX()->getEval()->evaluateY(-player);
	val2 = slimState->getBoardX()->getEval()->valueOfY(false, -player);
	if (slimState->getMoveNr() < 8) {
		// Native Unterscheidung für Bewertung
		if (this->currentPlayer == YPLAYER) {
			val1 = 0;
		}
		else {
			val2 = 0;
		}
	}
	return val2 - val1;
}
