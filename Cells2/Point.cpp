#include "Point.h"
#include "World.h"

long Point::lastID = 0;

Point::Point(WorldSettings* _ws, float tx, float ty, float tMass, bool tOwned)
{
	place.set(tx, ty);
	momentumAdded.set(0, 0);
	momentum.set(0, 0);

	ws = _ws;
	ws->setupMixure(particles, tMass);
	calcMass();
	calcRadius();
	for (int i = 0; i < WorldSettings::e_AmountOfParticles; i++)
	{
		particlesFlowing[i] = 0;
	}

	owned = tOwned;
	id = getNewID();
}
bool Point::combine(Point * other)
{
	if (Vector::getLength(place, other->place) < radiusCache + other->radiusCache)
	{
		throw "Code needs to be changed to use owned before it can be used";

		place.set(Vector(place, massCache, other->place, other->massCache));
		momentumAdded += other->momentumAdded;
		momentum += other->momentum;

		while (other->joints.size()>0)
		{
			if (!other->joints[0]->changeFromTo(other, this))
			{
				throw "Data Object Error";
			}
			joints.push_back(other->joints[0]);
			other->joints.erase(other->joints.begin());
		}

		for (int i = 0; i < WorldSettings::e_AmountOfParticles; i++)
		{
			particles[i] += other->particles[i];
		}
		calcMass();
		calcRadius();

		return true;
	}
	else
	{
		return false;
	}
}
Point::Point(Point * other, float ratio, bool tOwned)
{
	place.set(other->place);
	momentumAdded.set(0, 0);
	momentum.set(other->momentum * ratio);
	other->momentum.multiply(1 - ratio);

	ws = other->ws;
	for (int i = 0; i < WorldSettings::e_AmountOfParticles; i++)
	{
		particles[i] = other->particles[i] * ratio;
		other->particles[i] *= (1.0 - ratio);
		particlesFlowing[i] = 0;
	}
	other->calcMass();
	other->calcRadius();
	calcMass();
	calcRadius();

	owned = tOwned;
	id = getNewID();
}
Point::~Point()
{
	while(joints.size()>0)
	{
		joints[0]->deconstruct();
		joints.erase(joints.begin());
	}
}

void Point::addJoint(Joint * joint)
{
	joints.push_back(joint);
}
void Point::deleteJoint(const long& _id)
{
	bool found = false;
	for (int i = 0; i < joints.size(); i++)
	{
		if (joints[i]->getId() == _id)
		{
			found = true;
			joints.erase(joints.begin() + i--);
		}
	}
	if (!found)
	{
		throw "Jow";
	}
}

void Point::calcForceJoints()
{
	for (int i = 0; i < joints.size(); i++)
	{
		Vector unitPos(0.0, 0.0);
		if (id == joints[i]->getP1()->getID())
		{
			unitPos = Vector(joints[i]->getP2()->getPlace(), joints[i]->getP1()->getPlace());
		}
		else
		{
			unitPos = Vector(joints[i]->getP1()->getPlace(), joints[i]->getP2()->getPlace());
		}
		float currentLength = unitPos.getLength();
		if (currentLength != 0) {
			unitPos.devide(currentLength);
			float difference = joints[i]->getLength() - currentLength;
			unitPos.multiply(difference*joints[i]->getStrenth());
			momentumAdded += unitPos;
		}

		//dampening
		Vector unitVel(0.0, 0.0);
		if (id == joints[i]->getP1()->getID())
		{
			unitVel = Vector(joints[i]->getP1()->getVelocity(), joints[i]->getP2()->getVelocity());
		}
		else
		{
			unitVel = Vector(joints[i]->getP2()->getVelocity(), joints[i]->getP1()->getVelocity());
		}
		unitVel.multiply(joints[i]->getDamping());
		momentumAdded += unitVel;
	}
}
void Point::calcForcePoint(Point * other)
{
	Vector line(place, other->place);
	float radiusSum = radiusCache + other->radiusCache;
	if (line.isSmallerThen(radiusSum))
	{
		Vector force = line.getUnit() * radiusSum - line;
		momentumAdded += (force*-0.6);
		other->momentumAdded += (force*0.6);

		Vector velocityLine(getVelocity(), other->getVelocity());
		momentumAdded += (velocityLine*0.001);
		other->momentumAdded += (velocityLine*-0.001);
	}
}
void Point::calcForceLine(Line * line)
{
	Vector lineVector(line->getV1(), line->getV2());
	Vector linePerpendicular = lineVector.getPerpendicularClockwise().getUnit();
	Vector p1 = place + linePerpendicular * getRadius();
	Vector p2 = place - linePerpendicular * getRadius();
	Vector intersection(0.0, 0.0);
	if (Shapes::lineSegementsIntersect(p1, p2, line->getV1(), line->getV2(), intersection))
	{
		float l1 = Vector::getLength(intersection, p1);
		float l2 = Vector::getLength(intersection, p2);

		if (l1 > l2)
		{
			momentumAdded += (linePerpendicular * (2 * getRadius() - l2) * 0.1);
		}
		else
		{
			momentumAdded += (linePerpendicular * (-2 * getRadius() - l1) * 0.1);
		}
	}
}
void Point::applyForces(float precision)
{
	//forcesSum.multiply(precision);
	//forcesSum.devide(massCache);
	//velocity.add(forcesSum);
	momentum += momentumAdded;
	place.add(getVelocity()*precision);
	momentumAdded.set(0, 0);
}

void Point::calcMass()
{
	massCache = 0;
	for (int i = 0; i < WorldSettings::e_AmountOfParticles; i++)
	{
		massCache += ws->particleProtoType[i].unitMass * particles[i];
	}
}
void Point::calcRadius()
{
	radiusCache = sqrt(massCache / 3.1415926535897);
}

void Point::applyJointFlow(const float precision)
{
	if (joints.size() > 0)
	{
		for (int i = 0; i < WorldSettings::e_AmountOfParticles; i++)
		{
			particles[i] += particlesFlowing[i] * precision;
			if (particles[i] < 0.0)
			{
				throw "Anti Mater created";
			}
			particlesFlowing[i] = 0;
		}
		calcMass();
		calcRadius();
	}
}
bool Point::moveParticle(Point * other, const int i, const float t)
{
	if (t >= 0)
	{
		particlesFlowing[i] -= t;
		other->particlesFlowing[i] += t;

		Vector momentumDiff = momentum * (ws->particleProtoType[i].unitMass * t) / massCache;
		momentumAdded -= momentumDiff;
		other->momentumAdded += momentumDiff;
		return true;
	}
	else
	{
		return false;
	}
}
float Point::getParticleCount(const int i) const
{
	return particles[i];
}

const Vector& Point::getPlace()const
{
	return place;
}
const Vector& Point::getVelocity()const
{
	return momentum/massCache;
}
float Point::getMass()const
{
	return massCache;
}
float Point::getRadius()const
{
	return radiusCache;
}
int Point::getJointSize()const
{
	return joints.size();
}
Joint * Point::getJoint(int i)const
{
	return joints[i];
}

void Point::setOwned(bool tOwned)
{
	owned = tOwned;
}

bool Point::isOwned() const
{
	return owned;
}

long Point::getID()
{
	return id;
}
long Point::getNewID()
{
	return lastID++;
}