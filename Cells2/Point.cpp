#include "Point.h"
#include "World.h"

long Point::lastID = 0;

Point::Point()
{
	mass = 0;
}
Point::Point(const Point & other)
{
	forcesExtern.set(other.forcesExtern);
	forcesJoints.set(other.forcesJoints);
	place.set(other.place);
	velocity.set(other.velocity);
	mass = other.mass;
	id = other.id;
	for (auto joint : other.joints)
	{
		joints.push_back(joint);
	}
}

void Point::init(float tx, float ty, float tMass)
{
	place.set(tx, ty);
	mass = tMass;
	id = lastID;
	lastID++;
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

void Point::applyForces(float precision, float backgroundFriction, float newMass)
{
	mass = newMass;
	applyForces(precision, backgroundFriction);
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
}

int Point::getJointSize()const
{
	return joints.size();
}

Joint * Point::getJoint(int i)const
{
	return joints[i];
}

long Point::getID()
{
	return id;
}
