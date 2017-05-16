#include <iostream>
#include "Const.h"
#include "MatchUnit.h"
#include "Races.h"
#include "Zobrist.h"
#include "CheckPattern.h"
#include "InitialMoves.h"
#include "AlphaBeta.h"
#include "JNILogic.h"
#include "LinuxTimeUtils.h"
#include "OrderedMoves.h"
#include "Utilities.h"

using namespace std;

Move* getRandomMove(MatchUnit* mup) {
	vector<Move*>* pms = mup->generateAllPossibleMoves(mup->getCurrentPlayer());
	srand((int)time(NULL));
	Move* pm = pms->at(rand() % pms->size());
	for each (Move* m in *pms)
		if (m != pm)
			delete m;
	delete pms;
	return pm;
}

void initSingletons(MatchUnit* mup) {
	AlphaBeta::createInstance();
	Races::createInstance();
	Zobrist::createInstance();
	Zobrist::instance()->initialize();
	CheckPattern::createInstance();
	CheckPattern::instance()->loadPattern();
	InitialMoves::createInstance();
	OrderedMoves::staticInit(mup);
}

void initClient(MatchUnit* mup) {
	initSingletons(mup);
}

void integrityCheck() {
	cout << "=== INTEGRITY CHECK ===" << endl;
	for (int i = 0; i < 10; i++) {
		MatchUnit m;
		initClient(&m);
		Move ma[] = {  Move(22, 10),  Move(5, 6),  Move(6, 5),
			 Move(7, 10),  Move(9, 10),  Move(9, 10),  Move(15, 8),
			 Move(12, 9),  Move(20, 10),  Move(12, 17),  Move(21, 5),
			 Move(3, 9) };
		for each (Move move in ma)
			m.performMove(new Move(move));
		//Utilities::printBoard(&m);

		cout << endl;
		LinuxTimeUtils::ServerRequest();
		AlphaBeta::instance()->calculate(&m);
	}
	cout << "Integrity check finished." << endl;
	cout << "Press any key to start the server...";
}

void compare() {
	cout << "Starting comparison C++..." << endl;
	long a = 0, timeSum = 0, max = MIN_VALUE, min = MAX_VALUE, deltaTime;
	int iterations = 100;
	srand(1337);
	int ia[24][24];
	for (int x = 0; x < 24; x++)
		for (int y = 0; y < 24; y++)
			ia[x][y] = rand() % 2;

	for (int iter = 0; iter < iterations; iter++) {
		LinuxTimeUtils::ServerRequest();
		for (int i = 0; i < 500000; i++) {
			for (int x = 0; x < 24; x++) {
				for (int y = 0; y < 24; y++) {
					a *= ia[x][y];
				}
			}
		}
		deltaTime = LinuxTimeUtils::GetDeltaTime();
		timeSum += deltaTime;
		if (deltaTime > max)
			max = deltaTime;
		else if (deltaTime < min)
			min = deltaTime;
	}

	long avg = timeSum / iterations;
	cout << "C++: a = " << a << " | AVG: " << avg << "ms | MIN: " << min << "ms | MAX: " << max << "ms" << endl;
}

void main() {
		
	if (SPEEDTEST)
		compare();

	if(INTEGRITY_CHECK)
		integrityCheck();

	cout << "Starting TWIXT server..." << endl;

	cout << "Preparing default server...";
	MatchUnit* mup = new MatchUnit();
	cout << "Default server online." << endl;

	cout << "Initialising client emulator...";
	initClient(mup);
	cout << "Done." << endl;
	for (int turn = 0; turn < 60; turn++) {
		cout << "+++ Turn " << turn << " +++" << endl;
		cout << "Player " << mup->getCurrentPlayer() << " is calculating..." << endl;
		Move* mp = nullptr;
		LinuxTimeUtils::ServerRequest();
		if (mup->getCurrentPlayer() == YPLAYER)
			mp = getRandomMove(mup);
		else
			mp = AlphaBeta::instance()->calculate(mup);
		mup->performMove(mp); // Transfer move* to MatchUnit
		cout << "Player " << -mup->getCurrentPlayer() << " has performed (" << mp->x << "|" << mp->y << ")." << endl;
		//Utilities::printBoard(mup);
		cout << "Accessing next turn with players swapped..." << endl;
	}

	delete mup;

	cout << "TWIXT server closed." << endl;

	string s;
	cin >> s;
}


