#pragma once
#include <memory>

class Tail;

#include "Cell.h"
#include "TailEnd.h"
#include "Point.h"

class Tail
{
public:
	Tail(Cell* cell, TailEnd* tailEnd);

	Tail* expand();
	void collapse();
	void collapseFull();

	void setLength(float left, float right);

	double getSurface()const;
	float getVolume()const;

	virtual shared_ptr<Point> getFrontLeftPoint()const;
	virtual shared_ptr<Point> getFrontRightPoint()const;

private:
	

	Tail* front;
	Tail* back;

	enum PointIndex {
		p_left, p_right
	};

	enum JointIndex {
		j_left, j_leftCross, j_right, j_rightCross, j_back
	};
};