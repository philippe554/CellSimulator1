#pragma once

class Line;

#include <algorithm> 

#include "Vector.h"

using namespace std;

class Line {
public:
	Line(Vector&tv1, Vector&tv2);
	Line(double x1, double y1, double x2, double y2);

	void setV1(Vector&v);
	void setV2(Vector&v);

	void setV1(double x, double y);
	void setV2(double x, double y);

	Vector* getV1();
	Vector* getV2();

	double getLeft();
	double getRight();
	double getTop();
	double getBottom();

	void calcBox();

private:
	Vector v1;
	Vector v2;

	double left;
	double right;
	double top;
	double bottom;
};

