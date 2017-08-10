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
	void collapse(Tail* other);
	void collapseFull(Cell* cell, TailEnd* tailEnd);

	void setLength(float left, float right);

	double getSurface()const;
	float getVolume()const;

	std::shared_ptr<Point> points[2];
	Joint* joints[5];
};