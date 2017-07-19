#pragma once

class Cell;

#include "World.h"
#include "Vector.h"
#include "Point.h"
#include "Joint.h"
#include "Line.h"
#include "DNA.h"


class Cell {
public:
	Cell(shared_ptr<DNA> tDna, World*tWorld, Vector tCenter, double tRadius);
	~Cell();

	void calcJointForces(const Vector& flow);
	void movePoints(double precision, double backgroundFriction);

	void cellCellCollision(Cell * other);
	void cellCellForce(Cell * other );
	void lineCellCollision(Line * line);
	void lineCellForce(Vector & perpendicular1, Vector & perpendicular2);

	void waterContact();
	bool inWater(const Vector * vec)const;

	static bool lineSegementsIntersect(Vector & p, Vector & p2, Vector & q, Vector & q2, Vector & intersection, double precision=0.0);
	static bool pointInTriangle(Vector & A, Vector & B, Vector & C, Vector & P);

	bool broken()const;
	void reRefPoints();

	bool connectCells(Cell*other);
	void disconnectCells(int );

	double getVolume()const;
	double getTemperature()const;
	double getSurface()const;

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

	//void setID(int t);
	//int getID();

	//Cell* getConnectedCell(int i);

	/*void setSize(double t);
	double getSize();
	void multiplySize(double t);*/

	static const int amountEdges = 6;
private:
	shared_ptr<Point> center;
	shared_ptr<Point> edgePoints[amountEdges];
	vector<shared_ptr<Point>> tail;

	Joint* radiusJoints[amountEdges];
	Joint* edgeJoints[amountEdges];
	vector<Joint*> tailJoints;

	shared_ptr<DNA> dna;

	Cell*connectedCells[amountEdges];
	World*world;

	int id;
	double radius;
	double buoyancy = 1;
	double termalEnergy;

	static int idCounter;

	double tailCounter;
};


