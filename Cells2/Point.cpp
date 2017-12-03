#include "Point.h"
#include "World.h"

long Point::lastID = 0;

Point::Point()
{
	set = false;
	registered = false;
	for (int i = 0; i < WorldSettings::e_AmountOfParticles; i++)
	{
		particlesFlowing[i] = 0;
	}
}
bool Point::init(WorldSettings* _ws, float tx, float ty, float tMass, bool _owned)
{
	if (!set)
	{
		place.set(tx, ty);
		momentumAdded.set(0, 0);
		momentum.set(0, 0);

		ws = _ws;
		ws->setupMixure(particles, tMass);
		calcMass();
		calcRadius();

		owned = _owned;
		set = true;
		id = getNewID();
		return true;
	}
	else
	{
		throw "Mass Error";
		return false;
	}
}
bool Point::combine(Point * other)
{
	if (set && other->set && Vector::getLength(place, other->place) < radiusCache + other->radiusCache)
	{
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

		other->set = false;
		return true;
	}
	else
	{
		throw "Mass Error";
		return false;
	}
}
bool Point::split(Point * other, bool _owned, float ratio)
{
	if (set && !other->set)
	{
		other->place.set(place);
		other->momentumAdded.set(0, 0);
		other->momentum.set(momentum * ratio);
		momentum.multiply(1 - ratio);

		other->ws = ws;
		for (int i = 0; i < WorldSettings::e_AmountOfParticles; i++)
		{
			other->particles[i] = particles[i] * ratio;
			particles[i] *= (1.0 - ratio);
		}
		other->calcMass();
		other->calcRadius();
		calcMass();
		calcRadius();

		other->owned = _owned;
		other->set = true;
		other->id = getNewID();
		return true;
	}
	else
	{
		throw "Mass Error";
		return false;
	}
}
bool Point::splitAndDeconstruct(Point * p1, bool _owned1, Point * p2, bool _owned2, float radio)
{
	if (set && !p1->set && !p2->set)
	{
		//Optimalization possible by hardcoding these 2 functions
		split(p2, _owned2, radio);
		moveAndDeconstruct(p1, _owned1);
		return true;
	}
	else
	{
		throw "Mass Error";
		return false;
	}
}
bool Point::moveAndDeconstruct(Point * other, bool _owned)
{
	if (set && !other->set)
	{
		other->place.set(place);
		other->momentumAdded.set(momentumAdded);
		other->momentum.set(momentum);

		while (joints.size()>0)
		{
			if (!joints[0]->changeFromTo(this, other))
			{
				throw "Data Object Error";
			}
			other->joints.push_back(joints[0]);
			joints.erase(joints.begin());
		}

		other->ws = ws;
		for (int i = 0; i < WorldSettings::e_AmountOfParticles; i++)
		{
			other->particles[i] = particles[i];
		}
		other->calcMass();
		other->calcRadius();

		set = false;
		other->owned = _owned;
		other->set = true;
		other->id = lastID;
		lastID++;
		return true;
	}
	else
	{
		throw "Mass Error";
		return false;
	}
}
bool Point::deconstruct()
{
	set = false;
	while(joints.size()>0)
	{
		joints[0]->deconstruct();
		joints.erase(joints.begin());
	}
	return true;
}

void Point::addJoint(Joint * joint)
{
	joints.push_back(joint);
}
void Point::deleteJoint(const long& _id)
{
	for (int i = 0; i < joints.size(); i++)
	{
		if (joints[i]->getId() == _id)
		{
			joints.erase(joints.begin() + i);
			i--;
		}
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

bool Point::isSet() const
{
	return set;
}
bool Point::isOwned() const
{
	return owned;
}
bool Point::isRegistered() const
{
	return registered;
}
void Point::setRegistered(bool t)
{
	registered = t;
}
long Point::getID()
{
	return id;
}
long Point::getNewID()
{
	return lastID++;
}