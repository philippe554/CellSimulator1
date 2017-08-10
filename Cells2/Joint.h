#pragma once

#include <memory>

class Joint;

#include "Point.h"

using namespace std;

class Joint {
public:
	Joint(shared_ptr<Point> tp1, shared_ptr<Point> tp2, double tStrength, double tDamping, bool tfriction, int tid, int tBelongsTo);
	~Joint();

	Vector calcFriction(const Vector& flow);

	shared_ptr<Point> getP1();
	shared_ptr<Point> getP2();
	shared_ptr<Point> getOther(shared_ptr<Point> p);

	double getLength();
	void setLength(double t);
	void multiplyLength(double t);

	double getStrenth();
	void setStrenth(double t);

	double getDamping();
	void setDamping(double t);

	int getID();
	void setID(int t);

	int getBelongsTo();
	void setBelongsTo(int t);

	shared_ptr<Point>  p1;
	shared_ptr<Point>  p2;
	double length;
	double originalLength;
	double strength;
	double damping;
	bool friction;
	Vector frictionForce;
	int id;
	int belongsTo;
};

