#include "Point.h"
#include "World.h"

long Point::lastID = 0;

Point::Point()
{
}
void Point::init(double tx, double ty, double tMass)
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

void Point::clearJoints()
{
	while (joints.size()>0) {
		delete joints[0];
	}
}

void Point::addForce(Vector & f)
{
	forceAddLock.lock();
	forcesExtern += f;
	forceAddLock.unlock();
}

void Point::addForce(Vector * f)
{
	forceAddLock.lock();
	forcesExtern += f;
	forceAddLock.unlock();
}

void Point::addForce(double x, double y)
{
	forceAddLock.lock();
	forcesExtern.addX(x);
	forcesExtern.addY(y);
	forceAddLock.unlock();
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
		double currentLength = unitPos.getLength();
		if (currentLength != 0) {
			unitPos.devide(currentLength);
			double difference = joints[i]->getLength() - currentLength;
			unitPos.multiply(difference*joints[i]->getStrenth());
			forcesJoints.add(unitPos);
		}

		//dampening
		Vector unitVel(0.0, 0.0);
		if (id == joints[i]->getP1->getID())
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

void Point::applyForces(double precision, double backgroundFriction)
{
	forceAddLock.lock();
	Vector sum = forcesExtern;
	forcesExtern.set(0, 0);
	forceAddLock.unlock();

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

void Point::applyForces(double precision, double backgroundFriction, double newMass)
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

void Point::setPlace(Vector & v)
{
	place.set(v);
}

void Point::setVelocity(Vector & v)
{
	velocity.set(v);
}

double Point::getMass()const
{
	return mass;
}

void Point::setMass(double t)
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
