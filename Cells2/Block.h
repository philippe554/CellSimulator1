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

class Block
{
public:
	Block(World*tWorld, Chunk*tChunk, const int _cx, const int _cy, const int _bx, const int _by);
	~Block();

	void createCell(shared_ptr<DNA> _dna, Vector& place, float radius);
	void giveCell(Cell* _cell);

	void searchAndDeregisterPoint(Point* point);

	void stage1();
	void stage2();
	void stage3();
	void stage4();

	void calcJointForces();
	void calcParticlesForce();
	void pointLineForce(Point * point, Line * line)const;
	void cellCellCollision();

	void calcFlow();
	//Vector getFlow()const;

	void loadDefaultChunk();
	void addLine(const double x1, const double y1, const double x2, const double y2);
	
	//float getConcentrationPoint(const int& particle, const Vector& place)const;
	void addFrictionForce(const Vector& force);
	//float calcVolume() const override;

	vector<shared_ptr<DNA>> getDNA();

	const int getAmountOfCells()const;
	const int getAmountOfPoints()const;
	const int getAmountOfLines()const;

	Cell* getCell(int i)const;
	Point* getPoint(int i)const;
	Line* getLine(int i)const;

	int getbx()const;
	int getby()const;
	int getcx()const;
	int getcy()const;

private:
	vector<Cell*> cells;
	vector<Point*> points;
	vector<Line*> lines;

	Block* neighbours[8];
	bool neighboursSameChunk[8];
	Chunk* chunk;
	World* world;
	int bx;
	int by;
	int cx;
	int cy;

	void linkBlocks(int x, int y, int i1, int i2);

	//float getConcentrationHelper(Block* neighbour, const int& particle)const;

	Vector frictionForce;
};
