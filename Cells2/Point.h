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
	Point(const Point& other);
	void init(float tx, float ty, float tMass);

	void addJoint(Joint* joint);
	void deleteJoint(const long& _id);

	void addForce(Vector&f);
	void addForce(Vector*f);
	void addForce(float x,float y);
	void calcForcesJoints();
	void applyForces(float precision, float backgroundFriction);
	void applyForces(float precision, float backgroundFriction, float newMass);

	const Vector& getPlace()const;
	const Vector& getVelocity()const;

	void setPlace(const Vector&v);
	void setVelocity(const Vector&v);

	float getMass()const;
	void setMass(float t);

	int getJointSize()const;
	Joint* getJoint(int i)const;

	long getID();
private:
	vector<Joint*> joints;

	Vector forcesExtern;
	Vector forcesJoints;

	Vector place;
	Vector velocity;

	float mass;

	long id;
	static long lastID;
};
