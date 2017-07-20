#pragma once

#include <vector>
#include <memory>

class Block;

#include "Line.h"
#include "Cell.h"
#include "Water.h"
#include "DNA.h"

using namespace std;

class Block
{
public:
	Block(World*tWorld,Chunk*tChunk,const int _cx,const int _cy, const int _bx,const int _by);
	~Block();

	void calcJointForces();
	void movePoints(double precision, double backgroundFriction);
	void cellCellCollision();
	void doRestructure();
	void calcCollisionBorder();
	void calcCollisionChunk(const Block* chunk, const bool cellCell);

	void calcFlow();
	void moveFlow();

	void loadDefaultChunk();
	void addLine(const double x1, const double y1, const double x2, const double y2);
	bool lineSegementsIntersect(Vector&p, Vector&p2, Vector&q, Vector&q2, Vector&intersection, double precision);

	double getTemperature()const;
	double getPresure()const;
	double getMass()const;
	double getConcentration(const int& particle, const Vector& place)const;
	void addFrictionForce(const Vector& force);

	vector<shared_ptr<DNA>> getDNA();

	vector<Cell*> cells;
	vector<Line*> lines;

	Block* neighbours[8];
	bool neighboursSameChunk[8];
	Chunk* chunk;
	World* world;
	int bx;
	int by;
	int cx;
	int cy;

private:
	void linkBlocks(int x, int y, int i1, int i2);

	void moveFlowHelper(Block* neighbour, float total);

	float getConcentrationHelper(Block* neighbour, const int& particle)const;

	float totalParticles;
	float temperature;
	Vector flow;
	Vector frictionForce;

	enum particlesName
	{
		p_carbon, p_oxygen, p_nitrogen,
		e_AmountOfParticles
	};

	float particles[e_AmountOfParticles];
};