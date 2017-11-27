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

	for (int i = 0; i < 5; i++)
	{
		Point* p = new Point();
		p->init(&world->ws, _bx*world->ws.blockSize + (rand() % (int)(world->ws.blockSize*10.0)) / 10.0, _by*world->ws.blockSize + (rand() % (int)(world->ws.blockSize*10.0)) / 10.0, 1.0f, false);
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
		if (!point->isOwned())
		{
			delete point;
		}
	}
	for (auto line : lines)
	{
		delete line;
	}
}

void Block::createCell(shared_ptr<DNA> _dna, Vector & place, float radius)
{
	Cell* newCell = new Cell(_dna, &world->ws,place,radius);
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

void Block::searchAndDeregisterPoint(Point * point)
{
	int newbx = world->calcBlock(point->getPlace().getX());
	int newby = world->calcBlock(point->getPlace().getY());
	Block* newBlock = world->findBlock_B(newbx, newby);
	if (newBlock != nullptr)
	{
		for (int i = 0; i < newBlock->points.size(); i++)
		{
			if (newBlock->points[i]->getID() == point->getID())
			{
				newBlock->points[i]->setRegistered(false);
				newBlock->points.erase(newBlock->points.begin() + i);
				return;
			}
		}
	}
	throw "ERROR!";
}

void Block::linkBlocks(int x, int y, int i1, int i2)
{
	neighbours[i1] = chunk->findBlock_B(x, y);
	if (neighbours[i1] != nullptr)
	{
		neighbours[i1]->neighbours[i2] = this;
		neighboursSameChunk[i1] = true;
	}
	else
	{
		neighbours[i1] = world->findBlock_B(x, y);
		if (neighbours[i1] != nullptr)
		{
			neighbours[i1]->neighbours[i2] = this;
			neighboursSameChunk[i2] = false;
		}
		else
		{

		}
	}
}

void Block::stage1()
{
	for (auto cell : cells)
	{
		cell->cellLogic();
		cell->growJoints();
		//cell->cacheParameters();
	}
	//cacheParameters();
}
void Block::stage2()
{
	calcJointForces();
	calcParticlesForce();
}
void Block::stage3()
{	
	for (auto point : points)
	{
		point->applyForces(world->ws.precision, 0.0);
	}
}
void Block::stage4()
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
				points.erase(points.begin() + i);
				i--;
			}
		}
		else if (newcx != cx || newcy != cy)
		{
			Chunk* chunk = world->findChunk_C(newcx, newcy);
			if (chunk != nullptr)
			{
				int newbx = world->calcBlock(points.at(i)->getPlace().getX());
				int newby = world->calcBlock(points.at(i)->getPlace().getY());
				Block* newBlock = chunk->findBlock_B(newbx, newby);
				newBlock->points.push_back(points[i]);
			}
			points.erase(points.begin() + i);
			i--;
		}
		else
		{
			int newbx = world->calcBlock(points.at(i)->getPlace().getX());
			int newby = world->calcBlock(points.at(i)->getPlace().getY());
			if (bx != newbx || by != newby)
			{
				Block* newBlock = chunk->findBlock_B(newbx, newby);
				if (newBlock != nullptr)
				{
					newBlock->points.push_back(points.at(i));
				}
				points.erase(points.begin() + i);
				i--;
			}
		}
	}
	for (int i = 0; i < cells.size(); i++)
	{
		if (cells.at(i)->isBroken())
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
		}
		else
		{
			int newcx = world->calcChunk(cells.at(i)->getCenter().getX());
			int newcy = world->calcChunk(cells.at(i)->getCenter().getY());
			if (newcx >= world->ws.c_WorldBoundary || newcx<0 || newcy >= world->ws.c_WorldBoundary || newcy < 0)
			{
				for (int j = 0; j < cells[i]->getAmountOfPoints(); j++)
				{
					searchAndDeregisterPoint(cells[i]->getPoint(j));
				}
				delete cells.at(i);
				cells.erase(cells.begin() + i);
				i--;
				world->ws.stats_CellsBroken++;
			}
			else if (newcx != cx || newcy != cy)
			{
				Chunk* chunk = world->findChunk_C(newcx, newcy);
				if (chunk != nullptr)
				{
					chunk->giveCell(cells.at(i));
				}
				else
				{
					for (int j = 0; j < cells[i]->getAmountOfPoints(); j++)
					{
						searchAndDeregisterPoint(cells[i]->getPoint(j));
					}
					delete cells.at(i);
				}
				cells.erase(cells.begin() + i);
				i--;
			}
			else
			{
				int newbx = world->calcBlock(cells.at(i)->getCenter().getX());
				int newby = world->calcBlock(cells.at(i)->getCenter().getY());
				if (bx != newbx || by != newby)
				{
					Block* newBlock = chunk->findBlock_B(newbx, newby);
					if (newBlock != nullptr)
					{
						newBlock->cells.push_back(cells.at(i));
					}
					cells.erase(cells.begin() + i);
					i--;
				}
				else
				{
					if (cells.at(i)->getStage() == 6)
					{
						vector<Cell*> newCells = cells.at(i)->split();
						for (int j = 0; j < cells[i]->getAmountOfPoints(); j++)
						{
							searchAndDeregisterPoint(cells[i]->getPoint(j));
						}
						delete cells.at(i);
						cells.erase(cells.begin() + i);
						i--;
						cells.push_back(newCells[0]);
						cells.push_back(newCells[1]);
					}
				}
			}
		}
	}
	for (int i = 0; i < points.size(); i++)
	{
		if (points[i]->isOwned() && !points[i]->isSet() && points[i]->isRegistered())
		{
			searchAndDeregisterPoint(points[i]);
			i--;
		}
	}
	for (int i = 0; i < cells.size(); i++)
	{
		for (int j = 0; j < cells[i]->getAmountOfPoints(); j++)
		{
			if (cells[i]->getPoint(j)->isSet() && !cells[i]->getPoint(j)->isRegistered())
			{
				points.push_back(cells[i]->getPoint(j));
				cells[i]->getPoint(j)->setRegistered(true);
			}
		}
	}
}

void Block::calcJointForces()
{
	for (auto cell : cells)
	{
		cell->calcJointForces();
	}
	/*Vector frictionTotal(0.0, 0.0);
	for(auto cell:cells)
	{
		frictionTotal.add(cell->calcJointForces(getFlow()));
	}
	
	if(frictionTotal.getX()>0)
	{
		if(neighbours[4]!=nullptr)
		{
			neighbours[4]->applyForce(0, -frictionForce.getX());
		}
	}
	else
	{
		applyForce(0, -frictionForce.getX());
	}
	if (frictionTotal.getY()>0)
	{
		if (neighbours[6] != nullptr)
		{
			neighbours[6]->applyForce(1, -frictionForce.getY());
		}
	}
	else
	{
		applyForce(1, -frictionForce.getY());
	}*/
}
void Block::calcParticlesForce()
{
	for (int i=0;i<points.size();++i)
	{
		//points[i]->addForce(Vector(0.0, 0.001)*points[i]->getMass());
		if (!world->ws.openCLOptimalization)
		{
			for (int j = i + 1; j < points.size(); ++j)
			{
				points[i]->calcForcePoint(points[j]);
			}
		}
		for (int j = 0; j < lines.size(); j++)
		{
			pointLineForce(points[i], lines[j]);
		}
		for (int k = 0; k < 4; k++)
		{
			if (neighbours[k] != nullptr)
			{
				if (!world->ws.openCLOptimalization)
				{
					for (int j = 0; j < neighbours[k]->points.size(); ++j)
					{
						points[i]->calcForcePoint(neighbours[k]->points[j]);
					}
				}
				for (int j = 0; j < neighbours[k]->lines.size(); j++)
				{
					pointLineForce(points[i], neighbours[k]->lines[j]);
				}
			}
		}
	}
}
void Block::pointLineForce(Point* point, Line* line)const
{
	Vector lineVector(line->getV1(), line->getV2());
	Vector linePerpendicular = lineVector.getPerpendicularClockwise().getUnit();
	Vector p1 = point->getPlace() + linePerpendicular * point->getRadius();
	Vector p2 = point->getPlace() - linePerpendicular * point->getRadius();
	Vector intersection(0.0, 0.0);
	if (Shapes::lineSegementsIntersect(p1, p2, line->getV1(), line->getV2(), intersection))
	{
		float l1 = Vector::getLength(intersection, p1);
		float l2 = Vector::getLength(intersection, p2);

		if (l1 > l2)
		{
			point->addForce(linePerpendicular * (2 * point->getRadius() - l2) * 0.1);
		}
		else
		{
			point->addForce(linePerpendicular * (-2 * point->getRadius() - l1) * 0.1);
		}
	}
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

void Block::addFrictionForce(const Vector& force)
{
	frictionForce -= force;
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


