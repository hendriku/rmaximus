#include <iostream>
#include <ctime>
#include <string>
#include "AlphaBeta.h"
#include "JNILogic.h"
#include "MatchUnit.h"
#include "Zobrist.h"
#include "Races.h"
#include "CheckPattern.h"
#include "InitialMoves.h"
#include "OrderedMoves.h"
#include "LinuxTimeUtils.h"

using namespace std;

MatchUnit* m;

void initSingletons() {
	AlphaBeta::createInstance();
	Races::createInstance();
	Zobrist::createInstance();
	Zobrist::instance()->initialize();
	CheckPattern::createInstance();
	CheckPattern::instance()->loadPattern();
	InitialMoves::createInstance();
}

void compareDLL() {
	cout << "Starting comparison DLL..." << endl;
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

JNIEXPORT void JNICALL Java_sc_rm_logic_JNILogic_initialize(JNIEnv* env, jobject logicJObject, jint jTurn, jint jCurrPlayer) {
	if (SPEEDTEST)
		compareDLL();

	jclass logicJClass = env->GetObjectClass(logicJObject);
	// gtPinVal: Methode die nur -2 für Swamp an (x,y) oder 0 für nichts zurückgibt
	jmethodID gtPinValueID = env->GetMethodID(logicJClass, "getPinValue", "(II)I");

	cout << "Initializing Singletons...";
	initSingletons();
	cout << "success.";

	cout << "Getting int turn... ";
	int turn = (jTurn == 1) ? 0 /*Gegner Zug muss noch ausgeführt werden*/ : jTurn;
	cout << "turn=" << turn << endl;

	cout << "Getting int currentPlayer... ";
	int currentPlayer = (jTurn == 1) ? -jCurrPlayer /*Gegner Zug muss noch ausgeführt werden*/ : jCurrPlayer;
	cout << "currentPlayer=" << currentPlayer << endl;

	cout << "Initializing boards... ";
	Board* realBoard = new Board();
	realBoard->zobristEnabled = true;
	Board* altBoard = new Board();

	// Die Swamps übernehmen
	int val;
	for (int y = 0; y < DIM; y++) {
		for (int x = 0; x < DIM; x++) {
			val = env->CallIntMethod(logicJObject, gtPinValueID, x, y);
			realBoard->fields[x + MARGIN][y + MARGIN].value = val;
			altBoard->fields[y + MARGIN][x + MARGIN].value = val;
		}
	}
	cout << "success." << endl;

	cout << "Initializing MatchUnit...";
	m = new MatchUnit(realBoard, altBoard, turn, currentPlayer);
	cout << "success." << endl;

	cout << "Initializing OrderedMoves...";
	OrderedMoves::staticInit(m);
	cout << "success." << endl;
}

JNIEXPORT void JNICALL Java_sc_rm_logic_JNILogic_calculateSendMove(JNIEnv* env, jobject thisJObject, jint lmx, jint lmy) {
	LinuxTimeUtils::ServerRequest();

	// Letzten Move ausführen
	cout << "Performing enemys last move on match unit..." << endl;
	if (lmx == -1) {
		cout << "There was no lastMove to perform before. :(" << endl;
	}
	else {
		cout << "Found lastMove (" << lmx << "|" << lmy << ")! Performing now...";
		m->performMove(new Move(lmx, lmy));
		cout << "LastMove successfully performed as " << -m->getCurrentPlayer() << " in turn " << m->getTurn() - 1 << "." << endl;
	}

	cout << "Calculating move (Lost time = " << LinuxTimeUtils::GetDeltaTime() << "ms)..." << endl;
	Move* sendMove = AlphaBeta::instance()->calculate(m);

	cout << "Performing calculated move (" << sendMove->x << "|" << sendMove->y << ")...";
	m->performMove(sendMove);
	cout << "Success!" << endl;


	// TODO Seperates Modul entwickeln in welchem man jederzeit einen Zug über die Schnittstelle senden kann.
	cout << "Sending move " << sendMove->x << " | " << sendMove->y << " to java client...";
	jclass logicJClass = env->GetObjectClass(thisJObject);
	jfieldID xFID = env->GetFieldID(logicJClass, "sendX", "I");
	jfieldID yFID = env->GetFieldID(logicJClass, "sendY", "I");
	env->SetIntField(thisJObject, xFID, sendMove->x);
	env->SetIntField(thisJObject, yFID, sendMove->y);
	cout << "Sent!" << endl;
}

JNIEXPORT void JNICALL Java_sc_rm_logic_JNILogic_gameEnded(JNIEnv* env, jobject thisJObject) {
	cout << "Game ended." << endl;
}
