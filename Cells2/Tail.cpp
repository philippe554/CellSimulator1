#include "Tail.h"

Tail::Tail(Cell* cell, TailEnd* tailEnd) {

}

Tail * Tail::expand()
{
	return nullptr;
}

void Tail::collapse()
{
}

void Tail::collapseFull()
{
}

void Tail::setLength(float left, float right)
{
	
}

double Tail::getSurface()const
{
	return joints[j_left].getRealLength() + joints[j_right].getRealLength();
}

float Tail::getVolume()const
{
	float t1 = Shapes::surfaceTriangle(points[p_left]->getPlace(), getFrontLeftPoint()->getPlace(), getFrontRightPoint()->getPlace());
	float t2 = Shapes::surfaceTriangle(points[p_left]->getPlace(), points[p_right]->getPlace(), getFrontRightPoint()->getPlace());
	return t1 + t2;
}

shared_ptr<Point> Tail::getFrontLeftPoint()const
{
	return front->points[p_left];
}

shared_ptr<Point> Tail::getFrontRightPoint()const
{
	return front->points[p_right];
}
