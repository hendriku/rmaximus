#include "Evaluation.h"
#include "Races.h"
#include "Move.h"

Evaluation::Evaluation(Board * board) {
	this->board = board;
	this->stackcnt = 0;
}

void Evaluation::evaluateY(int nextPlayer) {
	int ymax = board->getYsize();
	int ownPinIndex, pinCount, x, y, pval, zval;
	int relevantXpos, relevantYpos;
	for (y = 1; y < board->getYsize(); y++) {
		for (ownPinIndex = 0, pinCount = rowPinCount[y]; ownPinIndex < pinCount; ownPinIndex++) {
			x = ownPinX[y][ownPinIndex];
			data[x][y].setValue(BLOCKED_FIELD_VAL_MULT);
			data[x][y].setRel(NO_FATHER, NO_FATHER);
		}
	}
	for (y = 0; y < ymax; y++) {
		for (ownPinIndex = 0, pinCount = rowPinCount[y]; ownPinIndex < pinCount; ownPinIndex++) {
			x = ownPinX[y][ownPinIndex];
			pval = getDistVal(x, y);
			relevantXpos = data[x][y].getFatherX();
			relevantYpos = data[x][y].getFatherY();
			if (relevantXpos == BLOCKED_FIELD_VAL) {
				zval = getDistVal(x - 1, y - 2) + MALUS_1;
				if (board->pinAllowed(x - 1, y - 2, YPLAYER) && board->bridgeAllowed(x, y, 1) && zval < pval) {
					pval = zval;
					relevantXpos = data[x - 1][y - 2].getFatherX();
					relevantYpos = data[x - 1][y - 2].getFatherY();
				}
				zval = getDistVal(x + 1, y - 2) + MALUS_1;
				if (board->pinAllowed(x + 1, y - 2, YPLAYER) && board->bridgeAllowed(x, y, 2) && zval < pval) {
					pval = zval;
					relevantXpos = data[x + 1][y - 2].getFatherX();
					relevantYpos = data[x + 1][y - 2].getFatherY();
				}
				zval = getDistVal(x - 2, y - 1) + MALUS_2;
				if (board->pinAllowed(x - 2, y - 1, YPLAYER) && board->bridgeAllowed(x, y, 0) && zval < pval) {
					pval = zval;
					relevantXpos = data[x - 2][y - 1].getFatherX();
					relevantYpos = data[x - 2][y - 1].getFatherY();
				}
				else {
					zval = getDistVal(x + 2, y - 1) + MALUS_2;
					if (board->pinAllowed(x + 2, y - 1, YPLAYER) && board->bridgeAllowed(x, y, 3) && zval < pval) {
						pval = zval;
						relevantXpos = data[x + 2][y - 1].getFatherX();
						relevantYpos = data[x + 2][y - 1].getFatherY();
					}
				}
			}
			bool acceptFather = relevantYpos != 0 || ((checkPlausiTop(x, relevantXpos, relevantYpos, nextPlayer)) && Races::instance()->checkTop(x, y, board, nextPlayer));
			if (acceptFather && (pval < data[x][y].getValue() || y == 0)) {
				data[x][y].setRel(relevantXpos, relevantYpos);
				graphstX[0] = x;
				graphstY[0] = y;
				stackcnt = 1;
				evalStructure(pval);
			}
		}
	}
}

int Evaluation::valueOfY(bool bComputeCritical, int nextPlayer) {
	int bestVal = BLOCKED_FIELD_VAL_MULT;
	int checkX, checkY;
	int bestValTenth = bestVal / 10;
	int distTenth;
	int yBottom = board->getYsize() - 1;
	unordered_set<Move, Move::Hash> startingPoints;
	for (int xi = 1; xi < board->getXsize() - 1; xi++) {
		if (board->getPin(xi, yBottom) == YPLAYER) {
			if (data[xi][yBottom].getValue() <= bestVal)
				bestVal = data[xi][yBottom].getValue();
			if (bComputeCritical) {
				distTenth = data[xi][yBottom].getValue() / 10;
				if (distTenth < bestValTenth) {
					startingPoints.clear();
					bestValTenth = distTenth;
				}
				if (distTenth <= bestValTenth)
					startingPoints.insert(Move(xi, yBottom));
			}
		}
		else {
			int dist = getDistVal(xi, yBottom);
			distTenth = dist / 10;
			if (data[xi][yBottom].getFatherY() > 0 && distTenth <= bestValTenth) {
				checkX = data[xi][yBottom].getFatherX();
				checkY = data[xi][yBottom].getFatherY();
				if (checkPlausiBottom(xi, checkX, checkY, nextPlayer) && Races::instance()->checkButtom(checkX, checkY, board, nextPlayer)) {
					if (bComputeCritical) {
						if (distTenth < bestValTenth) {
							startingPoints.clear();
							bestValTenth = distTenth;
						}
						if (distTenth <= bestValTenth)
							startingPoints.insert(Move(checkX, checkY));
					}
					if (dist < bestVal)
						bestVal = dist;
				}
			}
		}
	}
	if (bComputeCritical) {
		if (!critPoss.empty())
			critPoss.clear();
		for (Move m : startingPoints)
			computeCritical(m.x, m.y);
	}
	return bestVal;
}

void Evaluation::setupForY() {
	int xmax = board->getXsize();
	int ymax = board->getYsize();
	int x, y;
	rowPinCount[0] = 0;
	for (x = 0; x < xmax; x++) {
		calculateDistForY(x, 0);
		if (board->getPin(x, 0) == YPLAYER) {
			ownPinX[0][rowPinCount[0]] = x;
			rowPinCount[0]++;
		}
	}
	for (y = 1; y < ymax; y++) {
		rowPinCount[y] = 0;
		for (x = 0; x < xmax; x++) {
			calculateDistForY(x, y);
			if (board->getPin(x, y) == YPLAYER) {
				ownPinX[y][rowPinCount[y]] = x;
				rowPinCount[y]++;
			}
		}
	}
}

void Evaluation::addForY(int xin, int yin) {
	int player = board->getPin(xin, yin);
	updateRows(xin, yin, player);
	if (player == YPLAYER) {
		ownPinX[yin][rowPinCount[yin]] = xin;
		rowPinCount[yin]++;
	}
}

void Evaluation::removeForY(int xin, int yin, int player) {
	updateRows(xin, yin, player);
	if (player == YPLAYER)
		rowPinCount[yin]--;
}

unordered_set<CritPos, CritPos::Hash>* Evaluation::getCritical() {
	return &critPoss;
}

void Evaluation::evalStructure(int val) {
	int x1, y1, x2, y2;
	while (stackcnt > 0) {
		stackcnt--;
		x1 = graphstX[stackcnt];
		y1 = graphstY[stackcnt];
		data[x1][y1].setValue(val);
		x2 = x1 + 1;
		y2 = y1 - 2;
		if (board->isConnected(x1, y1, x2, y2) && data[x2][y2].getValue() > val) {
			graphstX[stackcnt] = x2;
			graphstY[stackcnt] = y2;
			stackcnt++;
		}
		x2 = x1 + 2;
		y2 = y1 - 1;
		if (board->isConnected(x1, y1, x2, y2) && data[x2][y2].getValue() > val) {
			graphstX[stackcnt] = x2;
			graphstY[stackcnt] = y2;
			stackcnt++;
		}
		x2 = x1 + 2;
		y2 = y1 + 1;
		if (board->isConnected(x1, y1, x2, y2) && data[x2][y2].getValue() > val) {
			graphstX[stackcnt] = x2;
			graphstY[stackcnt] = y2;
			stackcnt++;
		}
		x2 = x1 + 1;
		y2 = y1 + 2;
		if (board->isConnected(x1, y1, x2, y2) && data[x2][y2].getValue() > val) {
			graphstX[stackcnt] = x2;
			graphstY[stackcnt] = y2;
			stackcnt++;
		}

		x2 = x1 - 1;
		y2 = y1 + 2;
		if (board->isConnected(x1, y1, x2, y2) && data[x2][y2].getValue() > val) {
			graphstX[stackcnt] = x2;
			graphstY[stackcnt] = y2;
			stackcnt++;
		}
		x2 = x1 - 2;
		y2 = y1 + 1;
		if (board->isConnected(x1, y1, x2, y2) && data[x2][y2].getValue() > val) {
			graphstX[stackcnt] = x2;
			graphstY[stackcnt] = y2;
			stackcnt++;
		}
		x2 = x1 - 2;
		y2 = y1 - 1;
		if (board->isConnected(x1, y1, x2, y2) && data[x2][y2].getValue() > val) {
			graphstX[stackcnt] = x2;
			graphstY[stackcnt] = y2;
			stackcnt++;
		}
		x2 = x1 - 1;
		y2 = y1 - 2;
		if (board->isConnected(x1, y1, x2, y2) && data[x2][y2].getValue() > val) {
			graphstX[stackcnt] = x2;
			graphstY[stackcnt] = y2;
			stackcnt++;
		}
	}
}

Pin Evaluation::getFatherConn(int xc, int yc) {
	int x1, y1, x2, y2;
	graphstX[0] = xc;
	graphstY[0] = yc;
	stackcnt = 1;
	while (stackcnt > 0) {
		stackcnt--;
		x1 = graphstX[stackcnt];
		y1 = graphstY[stackcnt];
		if (data[x1][y1].getRelX() > NO_FATHER)
			return Pin(x1, y1);
		data[x1][y1].setValue(-(BLOCKED_FIELD_VAL));
		x2 = x1 + 1;
		y2 = y1 - 2;
		if (board->isConnected(x1, y1, x2, y2) && data[x2][y2].getValue() > -(BLOCKED_FIELD_VAL)) {
			graphstX[stackcnt] = x2;
			graphstY[stackcnt] = y2;
			stackcnt++;
		}
		x2 = x1 + 2;
		y2 = y1 - 1;
		if (board->isConnected(x1, y1, x2, y2) && data[x2][y2].getValue() > -(BLOCKED_FIELD_VAL)) {
			graphstX[stackcnt] = x2;
			graphstY[stackcnt] = y2;
			stackcnt++;
		}
		x2 = x1 + 2;
		y2 = y1 + 1;
		if (board->isConnected(x1, y1, x2, y2) && data[x2][y2].getValue() > -(BLOCKED_FIELD_VAL)) {
			graphstX[stackcnt] = x2;
			graphstY[stackcnt] = y2;
			stackcnt++;
		}
		x2 = x1 + 1;
		y2 = y1 + 2;
		if (board->isConnected(x1, y1, x2, y2) && data[x2][y2].getValue() > -(BLOCKED_FIELD_VAL)) {
			graphstX[stackcnt] = x2;
			graphstY[stackcnt] = y2;
			stackcnt++;
		}

		x2 = x1 - 1;
		y2 = y1 + 2;
		if (board->isConnected(x1, y1, x2, y2) && data[x2][y2].getValue() > -(BLOCKED_FIELD_VAL)) {
			graphstX[stackcnt] = x2;
			graphstY[stackcnt] = y2;
			stackcnt++;
		}
		x2 = x1 - 2;
		y2 = y1 + 1;
		if (board->isConnected(x1, y1, x2, y2) && data[x2][y2].getValue() > -(BLOCKED_FIELD_VAL)) {
			graphstX[stackcnt] = x2;
			graphstY[stackcnt] = y2;
			stackcnt++;
		}
		x2 = x1 - 2;
		y2 = y1 - 1;
		if (board->isConnected(x1, y1, x2, y2) && data[x2][y2].getValue() > -(BLOCKED_FIELD_VAL)) {
			graphstX[stackcnt] = x2;
			graphstY[stackcnt] = y2;
			stackcnt++;
		}
		x2 = x1 - 1;
		y2 = y1 - 2;
		if (board->isConnected(x1, y1, x2, y2) && data[x2][y2].getValue() > -(BLOCKED_FIELD_VAL)) {
			graphstX[stackcnt] = x2;
			graphstY[stackcnt] = y2;
			stackcnt++;
		}
	}
	return Pin(-1, -1);
}

bool Evaluation::calculateDistForY(int x, int y) {
	bool pinFound = false;
	if (y <= 0) {
		if (y < 0)
			return false;
		data[x][y].setFather(x, 0);
		if (board->getPin(x, y) == YPLAYER)
			data[x][y].setValue(0);
	}
	else {
		int fatherX, fatherY = UNUSED;
		if (board->isBridged(x - 1, y, 3) || board->isBridged(x + 1, y, 0)) {
			fatherX = BLOCKED_FIELD_VAL;
		}
		else if (board->getPin(x, y - 1) == XPLAYER) {
			pinFound = true;
			fatherX = BLOCKED_FIELD_VAL;
		}
		else if (board->getPin(x, y - 1) == YPLAYER) {
			pinFound = true;
			fatherX = x;
			fatherY = y - 1;
		}
		else {
			fatherX = data[x][y - 1].getFatherX();
			fatherY = data[x][y - 1].getFatherY();
		}
		if (board->getPin(x - 1, y - 2) == YPLAYER && board->bridgeAllowed(x, y, 1)) { // left
			fatherX = x - 1;
			fatherY = y - 2;
		}
		else if (board->getPin(x + 1, y - 2) == YPLAYER && board->bridgeAllowed(x, y, 2)) { // right
			fatherX = x + 1;
			fatherY = y - 2;
		}
		else if (board->getPin(x - 2, y - 1) == YPLAYER && board->bridgeAllowed(x, y, 0)) { // right
			fatherX = x - 2;
			fatherY = y - 1;
		}
		else if (board->getPin(x + 2, y - 1) == YPLAYER && board->bridgeAllowed(x, y, 3)) { // right
			fatherX = x + 2;
			fatherY = y - 1;
		}
		data[x][y].setFather(fatherX, fatherY);
	}
	return pinFound;
}

void Evaluation::updateRows(int x, int y, int player) {
	for (int yi = y; yi < board->getYsize(); yi++)
		if (calculateDistForY(x, yi) && yi > y + 1)
			break;
	if (x >= 2)
		for (int yi = y - 1; yi < board->getYsize(); yi++)
			if (calculateDistForY(x - 1, yi) && yi > y + 2)
				break;
	if (x >= 3)
		calculateRow(x - 2, y);
	if (x < board->getXsize() - 2)
		for (int yi = y - 1; yi < board->getYsize(); yi++)
			if (calculateDistForY(x + 1, yi) && yi > y + 2)
				break;
	if (x < board->getXsize() - 3)
		calculateRow(x + 2, y);
	if (player == XPLAYER) {
		if (x < board->getXsize() - 3 && (board->getPin(x + 2, y + 1) == XPLAYER || board->getPin(x + 2, y - 1) == XPLAYER))
			calculateRow(x + 3, y);
		if (x >= 3 && (board->getPin(x - 2, y + 1) == XPLAYER || board->getPin(x - 2, y - 1) == XPLAYER))
			calculateRow(x - 3, y);
	}
}

void Evaluation::calculateRow(int xin, int yin) {
	for (int yi = yin - 1; yi < board->getYsize(); yi++)
		if (calculateDistForY(xin, yi) && yi > yin + 2)
			break;
}

int Evaluation::getDistVal(int x, int y) {
	int dv = BLOCKED_FIELD_VAL_MULT;
	if (x < 1 || y < 0 || x >= board->getXsize())
		return dv;
	FieldData& thisData = data[x][y];
	if (thisData.getFatherX() == BLOCKED_FIELD_VAL)
		return dv;
	if (thisData.getFatherY() == 0)
		dv = y * MULT_EVAL;
	else
		dv = (y - thisData.getFatherY()) * MULT_EVAL + data[thisData.getFatherX()][thisData.getFatherY()].getValue();
	return dv + (abs(thisData.getFatherX() - x) == 1 ? 1 : (abs(thisData.getFatherX() - x) == 2 ? 3 : 0));
}

void Evaluation::computeCritical(int xin, int yin) {
	int xc = xin, yc = yin;

	bool finish = false;
	FieldData* thisData;
	do {
		if (yc < board->getYsize() - 1 && board->getPin(xc, yc) == YPLAYER)
			critPoss.insert(CritPos(xc, yc, DOWN));
		Pin fp = getFatherConn(xc, yc);
		if (fp.getX() < 0) {
			finish = true;
		}
		else {
			if (fp.getY() > 0)
				critPoss.insert(CritPos(fp.getX(), fp.getY(), UP));
			thisData = &data[fp.getX()][fp.getY()];
			if (thisData->getRelX() < 0 || fp.getY() == 0) {
				finish = true;
			}
			else {
				xc = thisData->getRelX();
				yc = thisData->getRelY();
			}
		}

	} while (!finish);

}

bool Evaluation::checkPlausiTop(int col, int rx, int ry, int nextPlayer) {
	int vert = abs(col - rx);
	if (vert == 0)
		return true;
	if (vert >= 2 && nextPlayer == XPLAYER)
		return false;
	if (ry < 6)
		return true;
	if (vert == 1)
		return (board->getPin(rx, ry - 1) == XPLAYER || board->getPin(rx, ry - 2) == XPLAYER || board->getPin(rx, ry - 3) == XPLAYER || board->getPin(rx, ry - 4) == XPLAYER);
	else if (col == rx - 2)
		return (board->getPin(rx - 1, ry - 1) == XPLAYER || board->getPin(rx - 1, ry - 2) == XPLAYER || board->getPin(rx - 1, ry - 3) == XPLAYER || board->getPin(rx - 1, ry - 4) == XPLAYER);
	else if (col == rx + 2)
		return (board->getPin(rx + 1, ry - 1) == XPLAYER || board->getPin(rx + 1, ry - 2) == XPLAYER || board->getPin(rx + 1, ry - 3) == XPLAYER || board->getPin(rx + 1, ry - 4) == XPLAYER);
	return false;
}

bool Evaluation::checkPlausiBottom(int col, int rx, int ry, int nextPlayer) {
	int vert = abs(col - rx);
	if (vert == 0)
		return true;
	if (vert >= 2 && nextPlayer == XPLAYER)
		return false;
	if (ry > board->getXsize() - 6)
		return true;
	if (vert == 1)
		return (board->getPin(rx, ry + 1) == XPLAYER || board->getPin(rx, ry + 2) == XPLAYER || board->getPin(rx, ry + 3) == XPLAYER || board->getPin(rx, ry + 4) == XPLAYER);
	else if (col == rx - 2)
		return (board->getPin(rx - 1, ry + 1) == XPLAYER || board->getPin(rx - 1, ry + 2) == XPLAYER || board->getPin(rx - 1, ry + 3) == XPLAYER || board->getPin(rx - 1, ry + 4) == XPLAYER);
	else if (col == rx + 2)
		return (board->getPin(rx + 1, ry + 1) == XPLAYER || board->getPin(rx + 1, ry + 2) == XPLAYER || board->getPin(rx + 1, ry + 3) == XPLAYER || board->getPin(rx + 1, ry + 4) == XPLAYER);
	return false;
}
