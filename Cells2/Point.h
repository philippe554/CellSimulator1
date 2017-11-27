#pragma once
#include <memory>
#include <vector>
#include <mutex>

class Point;

#include "Vector.h"
#include "WorldSettings.h"
#include "Joint.h"

using namespace std;

class Point{
public:
	Point();
	void init(WorldSettings* _ws, float tx, float ty, float tMass, bool _owned);
	bool combine(Point* other);
	void split(Point* other, bool _owned, float ratio = 0.5); //leave negative to split fair

	void deconstruct();

	void addJoint(Joint* joint);
	void deleteJoint(const long& _id);

	void addForce(Vector&f);
	void addForce(Vector*f);
	void addForce(float x,float y);
	void calcForcesJoints();
	void calcForcePoint(Point* other);
	void applyForces(float precision, float backgroundFriction);

	const Vector& getPlace()const;
	const Vector& getVelocity()const;

	void setPlace(const Vector&v);
	void setVelocity(const Vector&v);

	float getParticleCount(const int i)const;
	float getMass()const;
	float getRadius()const;

	int getJointSize()const;
	Joint* getJoint(int i)const;

	bool isSet()const;//exists
	bool isOwned()const; //is part of a cell
	bool isRegistered()const; //is registered in block?
	void setRegistered(bool t);

	long getID();

private:
	void calcMass();
	void calcRadius();

	vector<Joint*> joints;

	Vector forcesExtern;
	Vector forcesJoints;

	Vector place;
	Vector velocity;

	WorldSettings* ws;
	float particles[WorldSettings::e_AmountOfParticles];
	float massCache;
	float radiusCache;

	bool set;
	bool owned;
	bool registered;

	long id;
	static long lastID;
};
