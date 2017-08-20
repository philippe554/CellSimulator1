#pragma once

class CellFrame;

#include "Reactor.h"
#include "Shapes.h"
#include "Tail.h"
#include "TailEnd.h"
#include "Point.h"

class CellFrame
{
public:
	CellFrame(World * _world, const Vector& tCenter, const float radius);
	~CellFrame();

	Vector calcJointForces(const Vector& flow);
	void movePoints(float precision, float backgroundFriction);

	void cellCellCollision(Cell * other);
	void cellCellForce(Cell * other);

	bool isBroken()const;

	Vector*getEdgePoint(const int i)const;
	Vector*getTailPoint(const int i, const int j)const;
	int getAmountOfEdgeEdges()const;
	int getAmountOfTailEdges()const;
	Joint* getEdgeEdge(int i)const;
	Joint* getTailEdge(int i)const;
	Vector* getCenter()const;

	long getId()const;

	static const int AmountOfEdges = 6;
	static const int MaxTailLength = 12;

protected:
	Point center;
	Point edgePoints[AmountOfEdges];
	Joint radiusJoints[AmountOfEdges];
	Joint edgeJoints[AmountOfEdges];

	int tailLength;
	Point tailPoints[2 * MaxTailLength];
	Joint tailJoints[5 * MaxTailLength];

	bool hasTailEnd;
	Point tailEndPoint;
	Joint tailEndJoints[2];

	CellFrame* connectedCells[AmountOfEdges];

	World*world;

	long id;
	static long idCounter;
};

/*
void lineCellCollision(Line * line);
void lineCellForce(Vector & perpendicular1, Vector & perpendicular2);

*/