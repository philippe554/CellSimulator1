#pragma once

class Cell;

#include "World.h"
#include "Vector.h"
#include "Point.h"
#include "Joint.h"
#include "Line.h"
#include "DNA.h"
#include "Membrane.h"


class Cell : public Reactor, public Shapes{
public:
	Cell(shared_ptr<DNA> tDna, World*tWorld, Vector tCenter, double tRadius);
	~Cell();

	void calcJointForces(const Vector& flow);
	void movePoints(double precision, double backgroundFriction);

	void cellCellCollision(Cell * other);
	void cellCellForce(Cell * other );
	void lineCellCollision(Line * line);
	void lineCellForce(Vector & perpendicular1, Vector & perpendicular2);

	bool isBroken()const;
	void reRefPoints();

	bool connectCells(Cell*other);
	void disconnectCells(int );

	Membrane* getOuterMembrane()const;

	double getSurface()const;

	int getId()const;

	double calcHeating(const double surounding);
	void applyForce(Vector&v);

	shared_ptr<DNA> getDNA();

	//void setEdgePoint(int i,Point*t);
	Vector*getEdgePoint(const int i)const;
	Vector*getTailPoint(const int i, const int j)const;
	int getEdgePointId(const int i)const;
	shared_ptr<Point> getEdgePointPtr(const int i)const;
	int getAmountOfEdgeEdges()const;
	int getAmountOfTailEdges()const;

	Joint* getEdgeEdge(int i)const;
	Joint* getTailEdge(int i)const;

	//void setCenter(Point*t);
	Vector* getCenter()const;
	int getCenterId()const;
	shared_ptr<Point> getCenterPtr()const;

	static const int amountEdges = 6;
private:
	shared_ptr<Point> center;
	shared_ptr<Point> edgePoints[amountEdges];
	vector<shared_ptr<Point>> tail;

	Joint* radiusJoints[amountEdges];
	Joint* edgeJoints[amountEdges];
	vector<Joint*> tailJoints;

	Membrane* outerMembrane;

	shared_ptr<DNA> dna;

	Cell*connectedCells[amountEdges];
	World*world;

	int id;
	double radius;

	static int idCounter;

	double tailCounter;
};


