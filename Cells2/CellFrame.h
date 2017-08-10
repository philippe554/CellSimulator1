#pragma once

class CellFrame;

#include "Reactor.h"
#include "Shapes.h"
#include "Tail.h"
#include "TailEnd.h"
#include "Point.h"

class CellFrame : public Reactor, public Shapes
{
public:
	CellFrame(World* _world);

	Vector calcJointForces(const Vector& flow);
	void movePoints(double precision, double backgroundFriction);

	void cellCellCollision(Cell * other);
	void cellCellForce(Cell * other);
	void lineCellCollision(Line * line);
	void lineCellForce(Vector & perpendicular1, Vector & perpendicular2);

	bool isBroken()const;
	void reRefPoints();

	void applyPressure(float p);
	double getSurface()const;
	float getVolume() const override;

	Vector*getEdgePoint(const int i)const;
	Vector*getTailPoint(const int i, const int j)const;
	int getAmountOfEdgeEdges()const;
	int getAmountOfTailEdges()const;
	Joint* getEdgeEdge(int i)const;
	Joint* getTailEdge(int i)const;
	Vector* getCenter()const;

	static const int AmountOfEdges = 6;
protected:
	shared_ptr<Point> center;
	shared_ptr<Point> edgePoints[AmountOfEdges];

	Joint* radiusJoints[AmountOfEdges];
	Joint* edgeJoints[AmountOfEdges];

	vector<Tail*> tail;

	bool hasTailEnd;
	shared_ptr<Point> tailEndPoint;
	Joint* tailEndJoints[2];

	CellFrame* connectedCells[AmountOfEdges];

	World*world;
};