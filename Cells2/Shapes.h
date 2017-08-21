#pragma once

class Shapes;

#include "Vector.h"

class Shapes
{
public:
	static bool lineSegementsIntersect(const Vector&p, const Vector&p2, const Vector&q, const Vector&q2, Vector&intersection, double precision = 0.0);
	static bool pointInTriangle(Vector & A, Vector & B, Vector & C, Vector & P);
	static float surfaceTriangle(float l1, float l2, float l3);
	static float surfaceTriangle(const Vector & A, const Vector & B, const Vector & C);

};
