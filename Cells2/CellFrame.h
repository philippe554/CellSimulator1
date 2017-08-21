#pragma once

class CellFrame;

#include "WorldSettings.h"
#include "Point.h"
#include "Reactor.h"
#include "Shapes.h"

class CellFrame : public Reactor
{
public:
	CellFrame(WorldSettings * _ws, const Vector& tCenter, const float radius);
	~CellFrame();

	Vector calcJointForces(const Vector& flow);
	void movePoints(float precision, float backgroundFriction);

	void cellCellCollision(CellFrame* other);
	void cellCellCollisionHelper(CellFrame* other);

	bool connectCells(CellFrame*other);
	void disconnectCells(int);

	void applyPressure(float p);
	double getSurface()const;
	float getVolume()const override;

	bool isBroken()const;

	const Vector& getEdgePoint(const int i)const;
	const Vector& getTailPoint(const int i, const int j)const;
	int getAmountOfEdgeEdges()const;
	int getAmountOfTailEdges()const;
	const Joint& getEdgeEdge(int i)const;
	const Joint& getTailEdge(int i)const;
	const Vector& getCenter()const;

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

	long id;
	static long idCounter;
};

/*
void lineCellCollision(Line * line);
void lineCellForce(Vector & perpendicular1, Vector & perpendicular2);

*/