#include "Field.h"

Field::Field() {
	Clear();
}

Field::Field(int val) {
	Clear();
	this->value = val;
}

void Field::Clear() {
	this->value = 0;
	this->bridge[0] = 0;
	this->bridge[1] = 0;
	this->bridge[2] = 0;
	this->bridge[3] = 0;
}
