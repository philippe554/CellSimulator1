#pragma once

class Water;

#include "Point.h"
#include "Line.h"
#include "World.h"

class Water
{
public:
	Water(World*tWorld,Vector tPlace,double tRadius);
	~Water();

	void waterWaterCollision(Water*other);
	void lineWaterCollision(Line*line);
	//void lineWaterCollisionCheck(Line*line)
	void postForcesCalc();

	double getWaterWaterForce(double distance);
	bool lineSegementsIntersect(Vector & p, Vector & p2, Vector & q, Vector & q2, Vector & intersection, double precision = 0.0);

	Point* getCenter();
	double getRadius();
private:
	Point* place;
	double radius;
	double buoyancy = 1;
	World*world;
};