#pragma once

class FieldData {
public:
	int value, fatherX, fatherY, relX, relY;
	
	FieldData();
	~FieldData();

	int getFatherX();
	int getFatherY();
	int getValue();
	int getRelX();
	int getRelY();
	void setFather(int,int);
	void setValue(int);
	void setRel(int,int);
};