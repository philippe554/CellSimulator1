#pragma once

#include <vector>
#include <memory>

class Block;

#include "Reactor.h"
#include "Shapes.h"
#include "Line.h"
#include "Cell.h"
#include "DNA.h"
#include "Chunk.h"

using namespace std;

class Block : public Reactor, public Shapes
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
	void cacheFlow();
	Vector getFlow()const;

	void loadDefaultChunk();
	void addLine(const double x1, const double y1, const double x2, const double y2);
	
	float getConcentrationPoint(const int& particle, const Vector& place)const;
	void addFrictionForce(const Vector& force);
	float getVolume() const override;

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

	float getConcentrationHelper(Block* neighbour, const int& particle)const;

	Vector flow;
	Vector frictionForce;
};
