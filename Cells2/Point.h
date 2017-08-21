#pragma once
#include <memory>
#include <vector>
#include <mutex>

class Point;

#include "Vector.h"
#include "Joint.h"

using namespace std;

class Point : public std::enable_shared_from_this<Point> {
public:
	Point();
	void init(double tx, double ty, double tMass);

	void addJoint(Joint* joint);
	void deleteJoint(const long& _id);

	void addForce(Vector&f);
	void addForce(Vector*f);
	void addForce(double x,double y);
	void calcForcesJoints();
private:
	void applyForces(double precision, double backgroundFriction);
public:
	void applyForces(double precision, double backgroundFriction, double newMass);

	const Vector& getPlace()const;
	const Vector& getVelocity()const;

	void setPlace(Vector&v);
	void setVelocity(Vector&v);

	double getMass()const;
	void setMass(double t);

	int getJointSize()const;
	Joint* getJoint(int i)const;

	long getID();
private:
	vector<Joint*> joints;

	Vector forcesExtern;
	Vector forcesJoints;

	Vector place;
	Vector velocity;

	double mass;

	long id;
	static long lastID;

	mutex forceAddLock;
};
