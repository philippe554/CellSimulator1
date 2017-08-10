#pragma once
class TailEnd;

#include "Point.h"

class TailEnd
{
public:
	double getSurface()const;
	float getVolume() const;

private:
	shared_ptr<Point> point;
	Joint* joints[2];
};