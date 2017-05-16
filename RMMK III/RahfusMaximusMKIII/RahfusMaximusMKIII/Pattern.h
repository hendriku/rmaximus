#pragma once
#include "PatternElement.h"
#include <string>
#include <vector>
#include "Board.h"
#include "CritPos.h"

using namespace std;

struct Pattern {
public:
	string name;
	vector<PatternElement*> elements;
	bool symmetric;

	Pattern(string text, bool sym);

	void addElement(PatternElement*);
	void checkPatternElements(CritPos*, Board*, int, bool);
};