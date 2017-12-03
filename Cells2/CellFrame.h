#pragma once

class CellFrame;

#include "WorldSettings.h"
#include "Point.h"
#include "Reactor.h"
#include "Shapes.h"

class CellFrame
{
public:
	CellFrame(WorldSettings * _ws, const Vector& tCenter, const float radius);
	CellFrame(CellFrame* parent);
	~CellFrame();

	void jointLogic();
	//Vector calcJointForces(const Vector& flow);
	void calcJointForces();
	void movePoints(float precision, float backgroundFriction);

	void cellCellCollision(CellFrame* other);
	void cellCellCollisionHelper(CellFrame * other, Joint & thisJoint);
	void cellCellCollisionHelper(CellFrame* other);
	
	bool connectCells(CellFrame*other);
	void disconnectCells(const int i);

	void startSplit(const int location);
	bool nextStage();
	int getStage();
	void growTail();

	void splitFrame(CellFrame* cell1, CellFrame* cell2);
	void splitFrameHelperCopyTail(CellFrame * newCell);
	void splitFrameHelperConnectCells(CellFrame* newCell, int own);

	void setTailFibers(int i, float left, float right, float cross);

	void applyPressure(float p);
	double getSurface()const;
	float calcVolume()const;

	bool isBroken()const;

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
	WorldSettings* ws;

	Point center;
	Point splitPoints[3];
	Point edgePoints[AmountOfEdges];
	Joint radiusJoints[AmountOfEdges];
	Joint edgeJoints[AmountOfEdges];
	Joint splitJoints[11];
	int splitPointsLength;
	int splitJointsLength;

	int tailLength;
	int tailLocation;
	Point tailPoints[2 * MaxTailLength];
	Joint tailJoints[5 * MaxTailLength];
	bool hasTailEnd;
	Point tailEndPoint;
	Joint tailEndJoints[4];

	bool connectedCellsMaster[AmountOfEdges];
	CellFrame* connectedCells[AmountOfEdges];
	Joint connectedCellsJoints[2 * AmountOfEdges];

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