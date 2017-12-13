#pragma once

class CellFrame;

#include "WorldSettings.h"
#include "Point.h"
#include "Reactor.h"
#include "Shapes.h"

class CellFrame
{
public:
	CellFrame(WorldSettings * _ws, const Vector& tCenter);
	CellFrame(CellFrame* parent);
	~CellFrame();

	void jointLogic();
	void calcJointForces();

	//bool connectCells(CellFrame*other);
	//void disconnectCells(const int i);

	void startSplit(const int location);
	void nextStage();
	int getStage();
	void growTail();

	void setTailFibers(int i, float left, float right, float cross);

	bool isBroken()const;
	Point* getNextNewPoint();

	Point* getPoint(int i);
	const Vector& getPointPlace(int i);
	const float getRadius(int i);
	const int getAmountOfPoints()const;

	const Vector& getEdgeJoint(const int i, const bool p1)const;
	const Vector& getRadiusJoint(const int i, const bool p1)const;
	const Vector& getSplitJoint(const int i, const bool p1)const;
	const Vector& getTailJoint(const int i, const bool p1) const;
	int getAmountOfEdgeJoints()const;
	int getAmountOfradiusJoints()const;
	int getAmountOfSplitJoints()const;
	int getAmountOfTailJoints()const;

	const Joint& getEdgeEdge(int i)const;
	const Joint& getTailEdge(int i)const;
	const Vector& getCenter()const;

	long getId()const;

	static const int AmountOfEdges = 6;
	static const int MaxTailLength = 8;

	int loc(int i);

protected:
	void registerPoint(Point** place, Point* point);

	WorldSettings* ws;

	Point* center;
	Point* splitPoints[3];
	Point* edgePoints[AmountOfEdges];
	Joint radiusJoints[AmountOfEdges];
	Joint edgeJoints[AmountOfEdges];
	Joint splitJoints[11];
	int splitPointsLength;
	int splitJointsLength;

	int tailLength;
	int tailLocation;
	Point* tailPoints[2 * MaxTailLength];
	Joint tailJoints[5 * MaxTailLength];
	bool hasTailEnd;
	Point* tailEndPoint;
	Joint tailEndJoints[4];

	//bool connectedCellsMaster[AmountOfEdges];
	//bool connectedToPoint[2 * AmountOfEdges];
	//CellFrame* connectedCells[AmountOfEdges];
	//Joint connectedCellsJoints[2 * AmountOfEdges];

	Joint armJoints[AmountOfEdges];

	vector<Point*> pointsToRegister;

	int stage;
	int splitLocation;
	float unit;

	long id;
	static long idCounter;
};

/*
void lineCellCollision(Line * line);
void lineCellForce(Vector & perpendicular1, Vector & perpendicular2);

*/