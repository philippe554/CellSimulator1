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
	Point();
	bool init(WorldSettings* _ws, float tx, float ty, float tMass, bool _owned);
	bool combine(Point* other);
	bool split(Point* other, bool _owned, float ratio = 0.5); //leave negative to split fair
	bool splitAndDeconstruct(Point* p1, bool _owned1, Point* p2, bool _owned2, float radio = 0.5);
	bool moveAndDeconstruct(Point* other, bool _owned);
	bool deconstruct();

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

	bool isSet()const;//exists
	bool isOwned()const; //is part of a cell
	bool isRegistered()const; //is registered in block?
	void setRegistered(bool t);

	long getID();
	long getNewID();

private:
	void calcMass();
	void calcRadius();

	vector<Joint*> joints;

	Vector place;
	Vector momentum;
	Vector momentumAdded;

	WorldSettings* ws;
	float particles[WorldSettings::e_AmountOfParticles];
	float particlesFlowing[WorldSettings::e_AmountOfParticles];
	float massCache;
	float radiusCache;

	bool set;
	bool owned;
	bool registered;

	long id;
	static long lastID;
};
