#pragma once
#include <memory>
#include <vector>
#include <mutex>

class Point;

#include "Vector.h"
#include "WorldSettings.h"
#include "Joint.h"
#include "Line.h"

using namespace std;

class Point{
public:
	Point(WorldSettings * _ws, float tx, float ty, float tMass, bool tOwned = false);
	bool combine(Point* other);
	Point(Point * other, float ratio, bool tOwned = false);
	~Point();

	void addJoint(Joint* joint);
	void deleteJoint(const long& _id);

	void calcForceJoints();
	void calcForcePoint(Point* other);
	void calcForceLine(Line * line);
	void applyForces(float precision);
	void applyJointFlow(const float precision);

	const Vector& getPlace()const;
	const Vector& getVelocity()const;

	float getParticleCount(const int i)const;
	bool moveParticle(Point* other, const int i, const float t);
	float getMass()const;
	float getRadius()const;

	int getJointSize()const;
	Joint* getJoint(int i)const;

	void setOwned(bool tOwned);
	bool isOwned()const;

	long getID();
	long getNewID();

private:
	void calcMass();
	void calcRadius();

	vector<Joint*> joints;

	Vector place;
	Vector momentum;
	Vector momentumAdded;

	float particles[WorldSettings::e_AmountOfParticles];
	float particlesFlowing[WorldSettings::e_AmountOfParticles];
	float massCache;
	float radiusCache;

	WorldSettings* ws;
	bool owned;
	long id;
	static long lastID;
};
