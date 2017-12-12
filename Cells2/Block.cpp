#include "Block.h"
#include "WorldSettings.h"

Block::Block(World*tWorld, Chunk*tChunk, const int _cx, const int _cy, const int _bx, const int _by)          
{
	world = tWorld;
	chunk = tChunk;

	bx = _bx;
	by = _by;
	cx = _cx;
	cy = _cy;

	linkBlocks(bx + 1, by, 0, 4);
	linkBlocks(bx + 1, by + 1, 1, 5);
	linkBlocks(bx, by + 1, 2, 6);
	linkBlocks(bx - 1, by + 1, 3, 7);

	linkBlocks(bx - 1, by, 4, 0);
	linkBlocks(bx - 1, by - 1, 5, 1);
	linkBlocks(bx, by - 1, 6, 2);
	linkBlocks(bx + 1, by - 1, 7, 3);

	float bs = world->ws.blockSize;

	for (int i = 0; i < 0; i++)
	{
		Point* p = new Point(&world->ws, _bx*world->ws.blockSize + (rand() % (int)(world->ws.blockSize*10.0)) / 10.0, _by*world->ws.blockSize + (rand() % (int)(world->ws.blockSize*10.0)) / 10.0, 1.0f);
		points.push_back(p);
	}

	/*int counter = 0;
	double sumTemp = 0;
	for (auto neighbour:neighbours)
	{
		if (neighbour != nullptr)
		{
			counter++;
			sumTemp += neighbour->getTemperature();
		}
	}
	if (counter == 0)
	{
		init(&world->ws, world->ws.blockSize*world->ws.blockSize, world->ws.defaultTemperature);
	}
	else
	{
		init(&world->ws, world->ws.blockSize*world->ws.blockSize, sumTemp / counter);
	}*/

	//loadDefaultChunk();
}
Block::~Block()
{
	for(auto cell :cells)
	{
		delete cell;
	}
	for (auto point : points)
	{
		//if (!point->isOwned())
		{
			delete point;
		}
	}
	for (auto line : lines)
	{
		delete line;
	}
}

void Block::createCell(shared_ptr<DNA> _dna, Vector & place)
{
	Cell* newCell = new Cell(_dna, &world->ws,place);
	cells.push_back(newCell);
	for (auto cell : cells)
	{
		if (cell != newCell)
		{
			cell->connectCells(newCell);
		}
	}
	for (auto neighbour : neighbours)
	{
		if (neighbour != nullptr)
		{
			for (auto cell : neighbour->cells)
			{
				if (cell != newCell)
				{
					cell->connectCells(newCell);
				}
			}
		}
	}
}
void Block::giveCell(Cell * _cell)
{
	cells.push_back(_cell);
}
void Block::givePoint(Point* _point)
{
	points.push_back(_point);
}

void Block::restructurePoints()
{
	for (int i = 0; i < points.size(); i++)
	{
		int newcx = world->calcChunk(points.at(i)->getPlace().getX());
		int newcy = world->calcChunk(points.at(i)->getPlace().getY());
		if (newcx >= world->ws.c_WorldBoundary || newcx < 0 || newcy >= world->ws.c_WorldBoundary || newcy < 0)
		{
			if (!points.at(i)->isOwned())
			{
				delete points.at(i);
				points.erase(points.begin() + i--);
			}
		}
		else if (newcx != cx || newcy != cy)
		{
			Chunk* chunk = world->findChunk_C(newcx, newcy);
			if (chunk != nullptr)
			{
				chunk->givePoint(points.at(i));
				points.erase(points.begin() + i--);
			}
			else
			{
				throw "Error";
			}
		}
		else
		{
			int newbx = world->calcBlock(points.at(i)->getPlace().getX());
			int newby = world->calcBlock(points.at(i)->getPlace().getY());
			if (bx != newbx || by != newby)
			{
				chunk->findBlock_B(newbx, newby)->points.push_back(points.at(i));
				points.erase(points.begin() + i--);
			}
		}
	}
}
void Block::restructureCells()
{
	for (int i = 0; i < cells.size(); i++)
	{
		bool outOfWorld = false;
		for (int j = 0; !outOfWorld && j < cells[i]->getAmountOfPoints(); j++)
		{
			int newcx = world->calcChunk(cells[i]->getPoint(j)->getPlace().getX());
			int newcy = world->calcChunk(cells[i]->getPoint(j)->getPlace().getY());
			if (newcx >= world->ws.c_WorldBoundary || newcx < 0 || newcy >= world->ws.c_WorldBoundary || newcy < 0)
			{
				delete cells[i];
				cells.erase(cells.begin() + i--);
				outOfWorld = true;
			}
		}
		if (!outOfWorld)
		{
			int newcx = world->calcChunk(cells.at(i)->getCenter().getX());
			int newcy = world->calcChunk(cells.at(i)->getCenter().getY());
			if (newcx != cx || newcy != cy)
			{
				Chunk* chunk = world->findChunk_C(newcx, newcy);
				if (chunk != nullptr)
				{
					chunk->giveCell(cells.at(i));
					cells.erase(cells.begin() + i--);
				}
				else
				{
					throw "Error";
				}
			}
			else
			{
				int newbx = world->calcBlock(cells.at(i)->getCenter().getX());
				int newby = world->calcBlock(cells.at(i)->getCenter().getY());
				if (bx != newbx || by != newby)
				{
					chunk->findBlock_B(newbx, newby)->cells.push_back(cells.at(i));
					cells.erase(cells.begin() + i--);
				}
			}
		}
	}
}

void Block::linkBlocks(int x, int y, int i1, int i2)
{
	neighbours[i1] = chunk->findBlock_B(x, y);
	if (neighbours[i1] != nullptr)
	{
		neighbours[i1]->neighbours[i2] = this;
		neighboursSameChunk[i1] = true;
		neighbours[i1]->neighboursSameChunk[i2] = true;
	}
	else
	{
		neighbours[i1] = world->findBlock_B(x, y);
		if (neighbours[i1] != nullptr)
		{
			neighbours[i1]->neighbours[i2] = this;
			neighboursSameChunk[i2] = false;
			neighbours[i1]->neighboursSameChunk[i2] = false;
		}
	}
}

void Block::stage1()
{
	for (auto cell : cells)
	{
		cell->cellLogic();
		cell->jointLogic(); //expand joints - Calc Flow
		cell->calcJointForces(); //calc force of joints
	}
	for (int i = 0; i<points.size(); ++i)
	{
		for (int j = 0; j < chunk->getAmountOfPoints(); j++)
		{
			points[i]->calcForcePoint(chunk->getPoint(j));
		}
		for (int k = 0; k < 8; k++)
		{
			Chunk* c = chunk->getNeighbour(k);
			if (c != nullptr)
			{
				for (int j = 0; j < c->getAmountOfPoints(); j++)
				{
					points[i]->calcForcePoint(c->getPoint(j));
				}
			}
		}
		for (int j = i + 1; j < points.size(); ++j)
		{
			points[i]->calcForcePoint(points[j]);
		}
		for (int j = 0; j < lines.size(); j++)
		{
			points[i]->calcForceLine(lines[j]);
		}
		for (int k = 0; k < 4; k++)
		{
			if (neighbours[k] != nullptr)
			{
				for (int j = 0; j < neighbours[k]->points.size(); ++j)
				{
					points[i]->calcForcePoint(neighbours[k]->points[j]);
				}
				for (int j = 0; j < neighbours[k]->lines.size(); j++)
				{
					points[i]->calcForceLine(neighbours[k]->lines[j]);
				}
			}
		}
	}
}
void Block::stage2()
{
	for (auto point : points)
	{
		point->applyJointFlow(world->ws.precision);//apply flow
		point->applyForces(world->ws.precision);//sum forces
	}
}
void Block::stage3()
{	
	for (int i = 0; i < cells.size(); i++)
	{
		if (cells.at(i)->readyToSplit())
		{
			Cell* newCell = cells.at(i)->split();
			if (newCell != NULL)
			{
				cells.push_back(newCell);
			}
		}

		Point* p = cells.at(i)->getNextNewPoint();
		while (p != nullptr)
		{
			points.push_back(p);
			p = cells.at(i)->getNextNewPoint();
		}

		/*if (cells.at(i)->isBroken())
		{
			for (int j = 0; j < cells[i]->getAmountOfPoints(); j++)
			{
				if (cells[i]->getPoint(j)->isRegistered())
				{
					searchAndDeregisterPoint(cells[i]->getPoint(j));
				}
			}
			delete cells.at(i);
			cells.erase(cells.begin() + i);
			i--;
			world->ws.stats_CellsBroken++;
		}*/
	}
	
}
void Block::stage4()
{
	//deregisterUnsetPoints();
	//registerSetPoints();

	restructureCells();
	restructurePoints();
}

void Block::loadDefaultChunk()
{
	double scale = 10;
	addLine(200/scale, 400 / scale, 400 / scale, 400 / scale);
	addLine(200 / scale, 0 / scale, 200 / scale, 400 / scale);
	addLine(400 / scale, 0 / scale, 400 / scale, 400 / scale);

	addLine(400 / scale, 200 / scale, 600 / scale, 400 / scale);
	addLine(600 / scale, 400 / scale, 700 / scale, 450 / scale);
	addLine(700 / scale, 450 / scale, 900 / scale, 500 / scale);
	addLine(900 / scale, 500 / scale, 1200 / scale, 500 / scale);
	
	addLine(300 / scale, 400 / scale, 350 / scale, 600 / scale);
	addLine(350 / scale, 600 / scale, 400 / scale, 750 / scale);
	addLine(400 / scale, 750 / scale, 450 / scale, 850 / scale);
	addLine(450 / scale, 850 / scale, 500 / scale, 900 / scale);
	addLine(500 / scale, 900 / scale, 550 / scale, 925 / scale);
	addLine(550 / scale, 925 / scale, 600 / scale, 925 / scale);
	addLine(600 / scale, 925 / scale, 650 / scale, 900 / scale);

	addLine(975 / scale, 500 / scale, 950 / scale, 600 / scale);
	addLine(950 / scale, 600 / scale, 900 / scale, 750 / scale);
	addLine(900 / scale, 750 / scale, 850 / scale, 850 / scale);
	addLine(850 / scale, 850 / scale, 800 / scale, 900 / scale);
	addLine(800 / scale, 900 / scale, 750 / scale, 925 / scale);
	addLine(750 / scale, 925 / scale, 700 / scale, 925 / scale);
	addLine(700 / scale, 925 / scale, 650 / scale, 900 / scale);

	addLine(1000 / scale, 500 / scale, 1000 / scale, 800 / scale);
	addLine(1030 / scale, 500 / scale, 1030 / scale, 800 / scale);
	addLine(1030 / scale, 500 / scale, 1070 / scale, 800 / scale);
	addLine(1100 / scale, 500 / scale, 1070 / scale, 800 / scale);
}
void Block::addLine(const double x1, const double y1, const double x2, const double y2)
{
	const double left = bx*world->ws.blockSize;
	const double top = by*world->ws.blockSize;
	const double right = left + world->ws.blockSize;
	const double bottom = top + world->ws.blockSize;
	Vector p1(left, top);
	Vector p2(right,top);
	Vector p3(right,bottom);
	Vector p4(left,bottom);
	Vector l1(x1, y1);
	Vector l2(x2, y2);
	bool intersectionFound = false;
	if ((left < x1&&x1 <= right&&top < y1&&y1 <= bottom) || (left < x2&&x2 <= right&&top < y2&&y2 <= bottom))
	{
		intersectionFound = true;
	}
	Vector intersection(0.0,0.0);
	if (Shapes::lineSegementsIntersect(l1, l2, p1, p2, intersection, 0))
	{
		if (l1.getY() < l2.getY()) {
			l1 = intersection;
		}
		else
		{
			l2 = intersection;
		}
		intersectionFound = true;
	}
	if (Shapes::lineSegementsIntersect(l1, l2, p2, p3, intersection, 0))
	{
		if (l1.getX() > l2.getX()) {
			l1 = intersection;
		}
		else
		{
			l2 = intersection;
		}
		intersectionFound = true;
	}
	if (Shapes::lineSegementsIntersect(l1, l2, p3, p4, intersection, 0))
	{
		if (l1.getY() > l2.getY()) {
			l1 = intersection;
		}
		else
		{
			l2 = intersection;
		}
		intersectionFound = true;
	}
	if (Shapes::lineSegementsIntersect(l1, l2, p4, p1, intersection, 0))
	{
		if (l1.getX() < l2.getX()) {
			l1 = intersection;
		}
		else
		{
			l2 = intersection;
		}
		intersectionFound = true;
	}
	if(intersectionFound)
	{
		lines.push_back(new Line(l1.getX(), l1.getY(), l2.getX(), l2.getY()));
	}
}

vector<shared_ptr<DNA>> Block::getDNA()
{
	vector<shared_ptr<DNA>> data;
	for(auto cell : cells)
	{
		data.push_back(cell->getDNA());
	}
	return data;
}

const int Block::getAmountOfCells() const
{
	return cells.size();
}
const int Block::getAmountOfPoints() const
{
	return points.size();
}
const int Block::getAmountOfLines() const
{
	return lines.size();
}
Cell * Block::getCell(int i) const
{
	return cells[i];
}
Point * Block::getPoint(int i) const
{
	return points[i];
}
Line * Block::getLine(int i) const
{
	return lines[i];
}

int Block::getbx() const
{
	return bx;
}
int Block::getby() const
{
	return by;
}
int Block::getcx() const
{
	return cx;
}
int Block::getcy() const
{
	return cy;
}


