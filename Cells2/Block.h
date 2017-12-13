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

	void createCell(shared_ptr<DNA> _dna, Vector& place);
	void giveCell(Cell* _cell);
	void givePoint(Point * _point);

	void restructurePoints();
	void restructureCells();

	void stage1();
	void stage2();
	void stage3();
	void stage4();

	void loadDefaultChunk();
	void addLine(const double x1, const double y1, const double x2, const double y2);
	
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
