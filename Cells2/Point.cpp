#include "Point.h"
#include "World.h"

long Point::lastID = 0;

Point::Point()
{
	set = false;
	registered = false;
}


void Point::init(WorldSettings* _ws, float tx, float ty, float tMass, bool _owned)
{
	place.set(tx, ty);
	forcesExtern.set(0, 0);
	forcesJoints.set(0, 0);
	velocity.set(0, 0);
	
	ws = _ws;
	ws->setupMixure(particles, tMass);
	calcMass();
	calcRadius();

	owned = _owned;
	registered = false;
	set = true;
	id = lastID;
	lastID++;
}

bool Point::combine(Point * other)
{
	if (Vector::getLength(place, other->place) < radiusCache + other->radiusCache)
	{
		place.set(Vector(place, massCache, other->place, other->massCache));
		forcesExtern.set(Vector(forcesExtern, massCache, other->forcesExtern, other->massCache));
		forcesJoints.set(Vector(forcesJoints, massCache, other->forcesJoints, other->massCache));
		velocity.set(Vector(velocity, massCache, other->velocity, other->massCache));

		for (int i = 0; i < WorldSettings::e_AmountOfParticles; i++)
		{
			particles[i] += other->particles[i];
		}

		calcMass();
		calcRadius();

		while (other->joints.size()>0)
		{
			//joints[0]->deconstruct();
			//joints.erase(joints.begin());
		}

		other->set = false;
		return true;
	}
	else
	{
		return false;
	}
}

void Point::split(Point * other, bool _owned, float ratio)
{
	other->place.set(place);
	other->forcesExtern.set(0, 0);
	other->forcesJoints.set(0, 0);
	other->velocity.set(velocity);

	other->ws = ws;
	for (int i = 0; i < WorldSettings::e_AmountOfParticles; i++)
	{
		other->particles[i] = particles[i] * ratio;
		particles[i] *= (1.0-ratio);
	}
	other->calcMass();
	other->calcRadius();
	calcMass();
	calcRadius();

	other->owned = _owned;
	other->registered = false;
	other->set = true;
	other->id = lastID;
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
		addForce(velocityLine*0.001);
		other->addForce(velocityLine*-0.001);
	}
}

void Point::applyForces(float precision, float backgroundFriction)
{
	Vector sum = forcesExtern;
	forcesExtern.set(0, 0);

	sum += forcesJoints;
	sum.multiply(precision);
	sum.devide(massCache);
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

float Point::getParticleCount(const int i) const
{
	return particles[i];
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
