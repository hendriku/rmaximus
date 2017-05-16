#pragma once
#include <string>

using namespace std;

struct CritPos {
	CritPos(int, int, bool);

	bool isDir();
	int getX();
	int getY();

	bool operator==(const CritPos& m) const;
	bool operator==(const CritPos* that) const;

	struct Hash {
		size_t operator()(const CritPos& m) const;
	};
private:
	int x, y;
	bool dir;
};