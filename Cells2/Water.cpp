#include "Water.h"
#include "World.h"

Water::Water(World*tWorld,Vector tPlace,double tRadius)
{
	world = tWorld;
	radius = tRadius;
	//place = new Point(world,tPlace, radius*radius*3.1415926535897*buoyancy);
	throw "this func shouldnt be used :P";
}
Water::~Water()
{
	delete place;
}

void Water::waterWaterCollision(Water*other)
{
	Vector line(place->getPlace(), other->place->getPlace());
	double force = getWaterWaterForce(line.getLength());
	line.makeUnit();
	place->addForce(line*(-1*force*place->getMass()*0.1));
	other->place->addForce(line*(force*place->getMass()*0.1));
}
void Water::lineWaterCollision(Line*line)
{
	Vector line1(line->getV1(), line->getV2());
	Vector perpendicular = line1.getPerpendicularClockwise();
	Vector intersection(0.0, 0.0);
	if (this->lineSegementsIntersect(*line->getV1(), *line->getV2(), *place->getPlace() + (perpendicular * 10000), *place->getPlace() - (perpendicular * 10000), intersection))
	{
		Vector line2(intersection,*place->getPlace());
		double distance = line2.getLength();
		if(distance<radius)
		{
			line2.makeUnit();
			//double strength = (-distance + radius)*place->getMass();
			double strength = (1.0/distance)*place->getMass();
			place->addForce(line2*strength);
		}
	}
}
void Water::postForcesCalc()
{
	//place->addForce(Vector(0.0, world->c_Gravity*place->getMass()));
	//place->applyForces(world->c_JointSmoother, 0.1);
	//place->clearForces();
	throw "Old code";
}

double Water::getWaterWaterForce(double distance)
{
	distance /= radius;
	if(distance<=4)
	{
		return (7.08-5.74*distance+0.977*distance*distance)*radius;
	}else
	{
		return 0;
	}
}
bool Water::lineSegementsIntersect(Vector&p, Vector&p2, Vector&q, Vector&q2, Vector&intersection, double precision)
{
	Vector r = p2 - p;
	Vector s = q2 - q;
	double rxs = r.cross(s);
	if (rxs > 1e-5 || -rxs > 1e-5)
	{
		Vector p_q = q - p;
		double t = p_q.cross(s) / rxs;
		double u = p_q.cross(r) / rxs;
		if ((0 + precision < t && t < 1 - precision) && (0 + precision < u && u < 1 - precision))
		{
			intersection = p + (r*t);
			return true;
		}
	}
	return false;
}

Point* Water::getCenter()
{
	return place;
}
double Water::getRadius()
{
	return radius;
}
