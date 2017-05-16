#include "Pattern.h"
#include "Move.h"
#include "CheckPattern.h"
#include <iostream>
#include <string>
#include <sstream>

Pattern::Pattern(string text, bool sym) {
	name = text;
	symmetric = sym;
}

void Pattern::addElement(PatternElement* patternElement) {
	elements.push_back(patternElement);
}

void Pattern::checkPatternElements(CritPos* critPos, Board* board, int player, bool offense) {
	int px = -1;
	int py = -1;
	int hx, hy, ix, iy;
	bool okay = true;

	for (auto& element : elements) {
		hx = critPos->getX() + element->x1;
		hy = critPos->getY() + element->y1 * (critPos->isDir() ? 1 : -1);
		ix = critPos->getX() + element->x2;
		iy = critPos->getY() + element->y2 * (critPos->isDir() ? 1 : -1);
		if (hx < 0 || hy < 0 || hy > board->getYsize() || hx > board->getXsize()) {
			okay = false;
			break;
		}
		switch (element->condition) {
		case FREE_NR:
			okay = board->getPin(hx, hy) == 0;
			break;
		case OWN_NR:
			okay = board->getPin(hx, hy) == (offense ? YPLAYER : XPLAYER);
			break;
		case OPP_NR:
			okay = board->getPin(hx, hy) == (offense ? XPLAYER : YPLAYER);
			break;
		case SET_NR:
			px = hx;
			py = hy;
			okay = board->pinAllowed(px, py, (offense ? YPLAYER : XPLAYER));
			break;
		case BPOSS_NR:
			okay = board->isBridgeAllowed(hx, hy, ix, iy);
			break;
		case BEXIST_NR:
			okay = board->isConnected(hx, hy, ix, iy);
			break;
		case STRONG_NR:
			okay = board->isStrong(hx, hy);
			break;
		case NSTRONG_NR:
			okay = !board->isStrong(hx, hy);
			break;
		default:
			cout << "Error: " + to_string(element->condition) + " not handled in switch." << endl;
		}
		if (!okay)
			break;
	}
	if (okay && px >= 0) {
		if (offense)
			CheckPattern::instance()->moves->insert(Move(px, py, player));
		else
			CheckPattern::instance()->moves->insert(Move(py, px, player));
	}
}

