#pragma once

#include <memory>

class Joint;

#include "Point.h"

using namespace std;

class Joint {
public:
	Joint();
	void init(Point* tp1, Point* tp2, const float tStrength, const float tDamping, const bool tfriction);
	void init(Point * tp1, Point * tp2, const Joint & other);
	void deconstruct();
	bool changeFromTo(Point* from, Point* to);

	//Vector calcFriction(const Vector& flow);
	//void applyPresureForce(float p);
	void jointJointCollision(Joint * other);
	//void setFriction(bool _friction);

	Point* getP1()const;
	Point* getP2()const;
	Point* getOther(Point* p)const;

	float getLength();
	void setLength(float t);

	float getRealLength() const;

	void setTarget(const float _targetLength, const float _growFactor);
	void logic(const float precision);

	float getStrenth();
	void setStrenth(float t);

	float getDamping();
	void setDamping(float t);

	float getFlowRate(int i)const;
	void setFlowRate(const int i, const float t);

	//const Vector& getFrictionForce() const;

	long getId()const;

private:
	bool active;
	Point*  p1;
	Point*  p2;
	float length;
	float targetLength;
	float growFactor;
	float strength;
	float damping;
	float flowRate[WorldSettings::e_AmountOfParticles];
	//bool friction;
	//Vector frictionForce;
	int id;
	static long lastID;
};

