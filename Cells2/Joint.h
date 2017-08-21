#pragma once

#include <memory>

class Joint;

#include "Point.h"

using namespace std;

class Joint {
public:
	Joint();
	void init(Point* tp1, Point* tp2, const float tStrength, const float tDamping, const bool tfriction);
	void deconstruct();

	Vector calcFriction(const Vector& flow);

	Point* getP1()const;
	Point* getP2()const;
	Point* getOther(Point* p)const;

	float getLength();
	void setLength(float t);

	float getRealLength() const;

	float getStrenth();
	void setStrenth(float t);

	float getDamping();
	void setDamping(float t);

	const Vector& getFrictionForce() const;

	long getId()const;

private:

	Point*  p1;
	Point*  p2;
	float length;
	float originalLength;
	float strength;
	float damping;
	bool friction;
	Vector frictionForce;
	int id;
	static long lastID;
};

