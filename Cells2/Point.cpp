#include "Point.h"
#include "World.h"

long Point::lastID = 0;

Point::Point()
{
	set = false;
}


void Point::init(float tx, float ty, float tMass, bool _owned)
{
	place.set(tx, ty);
	forcesExtern.set(0, 0);
	forcesJoints.set(0, 0);
	velocity.set(0, 0);
	mass = tMass;
	calcRadius();
	owned = _owned;
	registered = false;
	set = true;
	id = lastID;
	lastID++;
}

void Point::deconstruct()
{
	set = false;
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
	for (int i = 0; i < joints.size(); i++)
	{
		if (joints[i]->getId() == _id)
		{
			joints.erase(joints.begin() + i);
			i--;
		}
	}
}

void Point::addForce(Vector & f)
{
	forcesExtern += f;
}

void Point::addForce(Vector * f)
{
	forcesExtern += f;
}

void Point::addForce(float x, float y)
{
	forcesExtern.addX(x);
	forcesExtern.addY(y);
}

void Point::calcForcesJoints()
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
			forcesJoints.add(unitPos);
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
		forcesJoints.add(unitVel);
	}
}

void Point::calcForcePoint(Point * other)
{
	Vector line(place, other->place);
	float radiusSum = radiusCache + other->radiusCache;
	if (line.isSmallerThen(radiusSum))
	{
		Vector force = line.getUnit() * radiusSum - line;
		addForce(force*-0.6);
		other->addForce(force*0.6);
		Vector velocityLine(velocity, other->velocity);
		addForce(velocityLine*0.6);
		other->addForce(velocityLine*-0.6);
	}
}

void Point::applyForces(float precision, float backgroundFriction)
{
	Vector sum = forcesExtern;
	forcesExtern.set(0, 0);

	sum += forcesJoints;
	//sum.addY(0.05*mass); // Gravity
	sum.multiply(precision);
	sum.devide(mass);
	velocity.add(sum);

	Vector friction = velocity.getUnit()*(backgroundFriction*precision);
	if (velocity.getLength()>friction.getLength())
	{
		velocity -= friction;
	}
	else
	{
		velocity.set(0, 0);
	}
	
	place.add(velocity*precision);
	forcesJoints.set(0, 0);
}

const Vector& Point::getPlace()const
{
	return place;
}

const Vector& Point::getVelocity()const
{
	return velocity;
}

void Point::setPlace(const Vector & v)
{
	place.set(v);
}

void Point::setVelocity(const Vector & v)
{
	velocity.set(v);
}

float Point::getMass()const
{
	return mass;
}

void Point::setMass(float t)
{
	mass = t;
	calcRadius();
}

float Point::getRadius()const
{
	return radiusCache;
}

void Point::calcRadius()
{
	radiusCache = sqrt(mass / 3.1415926535897);
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
