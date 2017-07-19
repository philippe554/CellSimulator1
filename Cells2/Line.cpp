#include "Line.h"

Line::Line(Vector & tv1, Vector & tv2)
	:v1(tv1), v2(tv2)
{
	calcBox();
}

Line::Line(double x1, double y1, double x2, double y2)
	: v1(x1, y1), v2(x2, y2)
{
	calcBox();
}

void Line::setV1(Vector & v)
{
	v1 = v;
	calcBox();
}

void Line::setV2(Vector & v)
{
	v2 = v;
	calcBox();
}

void Line::setV1(double x, double y)
{
	v1.set(x, y);
	calcBox();
}

void Line::setV2(double x, double y)
{
	v2.set(x, y);
	calcBox();
}

Vector * Line::getV1()
{
	return &v1;
}

Vector * Line::getV2()
{
	return &v2;
}

double Line::getLeft()
{
	return left;
}

double Line::getRight()
{
	return right;
}

double Line::getTop()
{
	return top;
}

double Line::getBottom()
{
	return bottom;
}

void Line::calcBox()
{
	left = min(v1.getX(), v2.getX());
	right = max(v1.getX(), v2.getX());
	top = min(v1.getY(), v2.getY());
	bottom = max(v1.getY(), v2.getY());
}
