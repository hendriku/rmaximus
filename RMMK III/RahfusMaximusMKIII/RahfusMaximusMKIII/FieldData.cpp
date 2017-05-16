#include "FieldData.h"

FieldData::FieldData() {
	value = fatherX = fatherY = relX = relY = 0;
}

FieldData::~FieldData() {}

int FieldData::getFatherX() {
	return fatherX;
}

int FieldData::getFatherY() {
	return fatherY;
}

int FieldData::getValue() {
	return value;
}

int FieldData::getRelX() {
	return relX;
}

int FieldData::getRelY() {
	return relY;
}

void FieldData::setFather(int x, int y) {
	fatherX = x;
	fatherY = y;
}

void FieldData::setValue(int v) {
	value = v;
}

void FieldData::setRel(int x, int y) {
	relX = x;
	relY = y;
}
