#pragma once
#include <vector>
#include <string>
#include "Const.h"
#include "MatchUnit.h"
#include <Windows.h>
#include <iostream>
#include <string>

using namespace std;

class Utilities {
public:
	static vector<string>* split(string src, string regex);
	static int indexOf(string src, string key);
	static WORD getPinColor(int x, int y, MatchUnit* m);
	static void printBoard(MatchUnit* m);
};