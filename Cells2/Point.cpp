#include "Point.h"
#include "World.h"

long Point::lastID = 0;

Point::Point(Chunk*tChunk, double tx, double ty, double tMass, long _belongsTo)
:place(tx, ty),velocity(0.0,0.0),forcesExtern(0.0,0.0),forcesJoints(0.0,0.0)
{
	chunk = tChunk;
	mass = tMass;
	id = lastID;
	lastID++;
	refed = false;
	chunk->world->stats_PointsCreated++;
	belongsTo = _belongsTo;
}
shared_ptr<Point> Point::MakePoint(Chunk*tChunk, double tx, double ty, double tMass, long _belongsTo)
{
	auto p = make_shared<Point>(tChunk, tx, ty, tMass,_belongsTo);
	return p;
}
shared_ptr<Point> Point::MakePoint(shared_ptr<Point> p1, shared_ptr<Point> p2)
{
	p1 = p1->getReal();
	p2 = p2->getReal();
	auto p = make_shared<Point>(p1->chunk, 0, 0, p1->mass + p2->mass,p1->belongsTo);
	p->velocity = Vector(p1->velocity, p1->mass, p2->velocity, p2->mass);
	p->place = Vector(p1->place, p1->mass, p2->place, p2->mass);
	p->forcesExtern = p1->forcesExtern + p2->forcesExtern;
	while (p1->joints.size()>0)
	{
		if (p1->joints[0]->p1->getID() == p1->getID())
		{
			p1->joints[0]->p1 = p;
		}else{
			p1->joints[0]->p2 = p;
		}
		p->joints.push_back(p1->joints[0]);
		p1->joints.erase(p1->joints.begin());
	}
	while (p2->joints.size()>0)
	{
		if (p2->joints[0]->p1->getID() == p2->getID())
		{
			p2->joints[0]->p1 = p;
		}else{
			p2->joints[0]->p2 = p;
		}
		p->joints.push_back(p2->joints[0]);
		p2->joints.erase(p2->joints.begin());
	}
	return p;
}
Point::~Point()
{
	while (joints.size()>0) {
		delete joints[0];
	}
	chunk->world->stats_PointsDestroyed++;
}

shared_ptr<Point> Point::getSubPoint(double newMass, int _belongsTo, int _other)
{
	if(refed)
	{
		return ref->getSubPoint(newMass, _belongsTo, _other);
	}
	if(newMass>mass)
	{
		throw "Requested mass is more than available mass";
	}
	auto p = Point::MakePoint(chunk, place.getX(),place.getY(), newMass, _belongsTo);
	mass -= newMass;
	belongsTo = _other;
	p->velocity = velocity;
	vector<Joint*>::iterator i = joints.begin();
	while (i != joints.end())
	{
		if(_belongsTo == (*i)->belongsTo)
		{
			p->addJoint((*i));
			if ((*i)->p1->getID() == id)
			{
				(*i)->p1 = p;
			}
			if ((*i)->p2->getID() == id)
			{
				(*i)->p2 = p;
			}
			i = joints.erase(i);
		}else
		{
			i++;
		}
	}
	return p;
}

void Point::addJoint(Joint * joint)
{
	if(refed)
	{
		ref->addJoint(joint);
		return;
	}
	joints.push_back(joint);
}

void Point::deleteJointPointer(Joint * joint)
{
	//if (refed)
	//{
		//ref->deleteJointPointer(joint);
		//return;
	// }
	for (int i = 0; i < joints.size(); i++)
	{
		if (joints[i] == joint)
		{
			joints.erase(joints.begin() + i);
			i--;
		}
	}
}

void Point::clearJoints()
{
	if (refed)
	{
		ref->clearJoints();
		return;
	}
	while (joints.size()>0) {
		delete joints[0];
	}
}

void Point::addForce(Vector & f)
{
	if (refed)
	{
		ref->addForce(f);
		return;
	}
	forceAddLock.lock();
	forcesExtern += f;
	forceAddLock.unlock();
}

void Point::addForce(Vector * f)
{
	if (refed)
	{
		ref->addForce(f);
		return;
	}
	forceAddLock.lock();
	forcesExtern += f;
	forceAddLock.unlock();
}

void Point::addForce(double x, double y)
{
	if (refed)
	{
		ref->addForce(x,y);
		return;
	}
	forceAddLock.lock();
	forcesExtern.addX(x);
	forcesExtern.addY(y);
	forceAddLock.unlock();
}

void Point::calcForcesJoints()
{
	if (refed)
	{
		ref->calcForcesJoints();
		return;
	}
	for (int i = 0; i < joints.size(); i++)
	{
		Vector unitPos(0.0, 0.0);
		if (id == joints[i]->p1->getID())
		{
			unitPos = Vector(joints[i]->p2->getPlace(), joints[i]->p1->getPlace());
		}
		else
		{
			unitPos = Vector(joints[i]->p1->getPlace(), joints[i]->p2->getPlace());
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
		if (id == joints[i]->p1->getID())
		{
			unitVel = Vector(joints[i]->p1->getVelocity(), joints[i]->p2->getVelocity());
		}
		else
		{
			unitVel = Vector(joints[i]->p2->getVelocity(), joints[i]->p1->getVelocity());
		}
		unitVel.multiply(joints[i]->getDamping());
		forcesJoints.add(unitVel);
	}
}

void Point::applyForces(double precision, double backgroundFriction)
{
	if (refed)
	{
		ref->applyForces(precision, backgroundFriction);
		return;
	}

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

const bool Point::isRefed()
{
	return refed;
}

void Point::setRef(shared_ptr<Point> p)
{
	refed = true;
	ref = p;
}

shared_ptr<Point> Point::getReal()
{
	if(refed)
	{
		return ref->getReal();
	}
	return shared_from_this();
}

Vector * Point::getPlace()
{
	if (refed)
	{
		return ref->getPlace();
	}
	return &place;
}

Vector * Point::getVelocity()
{
	if (refed)
	{
		return ref->getVelocity();
	}
	return &velocity;
}

void Point::setPlace(Vector & v)
{
	if (refed)
	{
		ref->setPlace(v);
		return;
	}
	place.set(v);
}

void Point::setVelocity(Vector & v)
{
	if (refed)
	{
		ref->setVelocity(v);
		return;
	}
	velocity.set(v);
}

void Point::setPlace(Vector * v)
{
	if (refed)
	{
		ref->setPlace(v);
		return;
	}
	place.set(v);
}

void Point::setVelocity(Vector * v)
{
	if (refed)
	{
		ref->setVelocity(v);
		return;
	}
	velocity.set(v);
}

void Point::setPlace(double tx, double ty)
{
	if (refed)
	{
		ref->setPlace(tx,ty);
		return;
	}
	place.set(tx,ty);
}

void Point::setVelocity(double tx, double ty)
{
	if (refed)
	{
		ref->setVelocity(tx,ty);
		return;
	}
	velocity.set(tx, ty);
}

double Point::getMass()
{
	if (refed)
	{
		return ref->getMass();
	}
	return mass;
}

void Point::setMass(double t)
{
	if (refed)
	{
		ref->setMass(t);
		return;
	}
	mass = t;
}

void Point::multiplyMass(double t)
{
	if (refed)
	{
		ref->multiplyMass(t);
		return;
	}
	mass *= t;
}

int Point::getJointSize()
{
	if (refed)
	{
		return ref->getJointSize();
	}
	return joints.size();
}

Joint * Point::getJoint(int i)
{
	if (refed)
	{
		return ref->getJoint(i);
	}
	return joints[i];
}

long Point::getID()
{
	return id;
}

long Point::getBelongsTo()
{
	return belongsTo;
}

int Point::getNewID()
{
	lastID++;
	return lastID;
}
