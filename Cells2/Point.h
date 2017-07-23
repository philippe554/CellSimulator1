#pragma once
#include <memory>
#include <vector>
#include <mutex>

class Point;

#include "Vector.h"
#include "Joint.h"
#include "Chunk.h"
#include "World.h"

using namespace std;

class Point : public std::enable_shared_from_this<Point> {
public:
	Point(Chunk*tChunk, double tx, double ty, double tMass, long _belongsTo);
	static shared_ptr<Point> MakePoint(Chunk*tChunk, double tx, double ty, double tMass, long _belongsTo);
	static shared_ptr<Point> MakePoint(shared_ptr<Point> p1, shared_ptr<Point>p2);
	~Point();

	shared_ptr<Point> getSubPoint(double newMass, int jointBelongsTo, int _other);

	void addJoint(Joint* joint);
	void deleteJointPointer(Joint* joint);
	void clearJoints();

	void addForce(Vector&f);
	void addForce(Vector*f);
	void addForce(double x,double y);
	void calcForcesJoints();
	void applyForces(double precision, double backgroundFriction);
	void applyForces(double precision, double backgroundFriction, double newMass);

	Vector* getPlace();
	Vector* getVelocity();

	const bool isRefed();
	void setRef(shared_ptr<Point> p);
	shared_ptr<Point> getReal();

	void setPlace(Vector&v);
	void setVelocity(Vector&v);

	void setPlace(Vector*v);
	void setVelocity(Vector*v);

	void setPlace(double tx,double ty);
	void setVelocity(double tx, double ty);

	double getMass();
	void setMass(double t);
	void multiplyMass(double t);

	int getJointSize();
	Joint* getJoint(int i);

	long getID();
	long getBelongsTo();

	static int getNewID();

private:
	vector<Joint*> joints;

	Vector forcesExtern;
	Vector forcesJoints;

	Vector place;
	Vector velocity;

	double mass;

	long id;
	long belongsTo;

	static long lastID;

	Chunk*chunk;

	bool refed;
	shared_ptr<Point> ref;

	mutex forceAddLock;
};
