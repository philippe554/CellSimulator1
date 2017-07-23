#pragma once

class Shapes;

#include "Vector.h"

class Shapes
{
public:
	static bool lineSegementsIntersect(Vector&p, Vector&p2, Vector&q, Vector&q2, Vector&intersection, double precision = 0.0);
	static bool pointInTriangle(Vector & A, Vector & B, Vector & C, Vector & P);
	static float surfaceTriangle(float l1, float l2, float l3);

};
