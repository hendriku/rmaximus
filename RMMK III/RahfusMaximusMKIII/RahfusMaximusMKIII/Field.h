#pragma once

using namespace std;

struct Field {
	int value;
	int bridge[4];

	Field();
	Field(int val);

	void Clear();
};