#pragma once
#include <memory>
#include <vector>
#include <mutex>

class Point;

#include "Vector.h"
#include "Joint.h"

using namespace std;

class Point{
public:
	Point();
	void init(float tx, float ty, float tMass, bool _owned);
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

	float getMass()const;
	void setMass(float t);

	float getRadius()const;
	void calcRadius();

	int getJointSize()const;
	Joint* getJoint(int i)const;

	bool isSet()const;
	bool isOwned()const;
	bool isRegistered()const;
	void setRegistered(bool t);

	long getID();
private:
	vector<Joint*> joints;

	Vector forcesExtern;
	Vector forcesJoints;

	Vector place;
	Vector velocity;

	float mass;
	float radiusCache;

	bool set;
	bool owned;
	bool registered;

	long id;
	static long lastID;
};
