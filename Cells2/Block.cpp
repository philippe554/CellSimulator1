#include "Block.h"

Block::Block(World*tWorld, Chunk*tChunk, const int _cx, const int _cy, const int _bx, const int _by)
	:flow(0.0,0.0)
{
	world = tWorld;
	chunk = tChunk;

	bx = _bx;
	by = _by;
	cx = _cx;
	cy = _cy;
	
	linkBlocks(bx + 1, by, 0, 4);
	linkBlocks(bx + 1, by+1, 1, 5);
	linkBlocks(bx , by+1, 2, 6);
	linkBlocks(bx -1, by+1, 3, 7);

	linkBlocks(bx -1, by, 4, 0);
	linkBlocks(bx -1, by-1, 5, 1);
	linkBlocks(bx , by-1, 6, 2);
	linkBlocks(bx + 1, by-1, 7, 3);

	int counter=0;
	double sumTemp = 0;
	for(auto neighbour:neighbours)
	{
		if(neighbour!=nullptr)
		{
			counter++;
			sumTemp += neighbour->temperature;
		}
	}
	if(counter==0)
	{
		temperature = world->c_DefaultTemperature;
	}else
	{
		temperature = sumTemp / counter;
	}

	//loadDefaultChunk();

	totalParticles = world->c_DefaultChunkParticleCount * world->c_BlockSize* world->c_BlockSize;
	particles[p_carbon] = 400;
	particles[p_oxygen] = 100;
	particles[p_nitrogen] = 50;
}
Block::~Block()
{
	for(auto line: lines)
	{
		delete line;
	}
	for(auto cell :cells)
	{
		delete cell;
	}
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
	}
}

void Block::calcJointForces()
{
	for(auto cell:cells)
	{
		cell->calcJointForces(flow);
	}
}

void Block::movePoints(double precision, double backgroundFriction)
{
	for (auto cell : cells)
	{
		cell->movePoints(precision, backgroundFriction);
	}
}
void Block::cellCellCollision()
{
	vector<Cell*>::iterator i = cells.begin();
	vector<Cell*>::iterator end = cells.end();
	while (i != end)
	{
		//(*i)->waterContact();
		vector<Cell*>::iterator j = i+1;
		while (j!= end)
		{
			(*i)->cellCellCollision(*j);
			j++;
		}
		vector<Line*>::iterator k = lines.begin();
		while (k != lines.end())
		{
			(*i)->lineCellCollision(*k);
			k++;
		}
		k = lines.begin();
		while (k != lines.end())
		{
			(*i)->lineCellCollision(*k);
			k++;
		}
		i++;
	}
	calcCollisionBorder();
}
void Block::doRestructure()
{
	for (int i = 0; i < cells.size(); i++)
	{
		cells.at(i)->reRefPoints();
		if (cells.at(i)->broken())
		{
			delete cells.at(i);
			cells.erase(cells.begin() + i);
			i--;
			world->stats_CellsBroken++;
		}
		else
		{
			int newcx = world->calcChunk(cells.at(i)->getCenter()->getX());
			int newcy = world->calcChunk(cells.at(i)->getCenter()->getY());
			if (newcx >= world->c_WorldBoundary || newcx<0 || newcy >= world->c_WorldBoundary || newcy < 0)
			{
				delete cells.at(i);
				cells.erase(cells.begin() + i);
				i--;
				world->stats_CellsBroken++;
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
					delete cells.at(i);
				}
				cells.erase(cells.begin() + i);
				i--;
			}
			else
			{
				int newbx = world->calcBlock(cells.at(i)->getCenter()->getX());
				int newby = world->calcBlock(cells.at(i)->getCenter()->getY());
				if (bx != newbx || by != newby)
				{
					Block* newBlock = chunk->findBlock_B(newbx, newby);
					if (newBlock != nullptr) {
						newBlock->cells.push_back(cells.at(i));
					}
					cells.erase(cells.begin() + i);
					i--;
				}
			}
		}
	}
}
void Block::calcCollisionBorder()
{
	for (int i = 0; i<4; i++)
	{
		if (neighbours[i] != nullptr)
		{
			calcCollisionChunk(neighbours[i], true);
		}
	}
	for (int i = 4; i<8; i++)
	{
		if (neighbours[i] != nullptr)
		{
			calcCollisionChunk(neighbours[i], false);
		}
	}
}
void Block::calcCollisionChunk(const Block* block, const bool cellCell)
{
	for (int i = 0; i < cells.size(); i++)
	{
		if (cellCell) {
			for (int j = 0; j < block->cells.size(); j++)
			{
				cells.at(i)->cellCellCollision(block->cells.at(j));
			}
		}
		for (int j = 0; j < block->lines.size(); j++)
		{
			cells.at(i)->lineCellCollision(block->lines.at(j));
		}
	}
}

void Block::reflow()
{
	int flowCounter = 0;
	Vector flowSum(0.0, 0.0);
	for (int j = 0; j < 8; j+=2) 
	{
		if (neighbours[j] != nullptr) 
		{
			flowCounter++;
			double flowRate = (neighbours[j]->getPresure() - getPresure())*world->c_FlowRate;
			Vector flowSub((bx - neighbours[j]->bx)*flowRate, (by - neighbours[j]->by)*flowRate);
			flowSum += flowSub;
		}
	}
	if(flowCounter>0)
	{
		flow = (flow*0.25)+(flowSum * (0.75 * world->c_BlockSize / flowCounter));
	}
	else
	{
		flow.multiply(0.25);
	}
}
void Block::calcTemp()
{
	int j = 0;
	if (neighbours[j] != nullptr) {
		double tempDif = temperature - neighbours[j]->temperature;
		double particalRatio = totalParticles / neighbours[j]->totalParticles;
		temperature -= tempDif*world->c_FlowRate / particalRatio;
		neighbours[j]->temperature += tempDif*world->c_FlowRate*particalRatio;
	}
	j = 6;
	if(neighbours[j]!=nullptr){
		double tempDif = temperature - neighbours[j]->temperature;
		if (tempDif>0) {
			tempDif *= 5;
		}
		else
		{
			tempDif /= 5;
		}
		double particalRatio = totalParticles / neighbours[j]->totalParticles;
		temperature -= tempDif*world->c_FlowRate / particalRatio;
		neighbours[j]->temperature += tempDif*world->c_FlowRate*particalRatio;
	}
	for (auto cell : cells)
	{
		temperature += cell->calcHeating(temperature) / totalParticles;
		cell->applyForce(flow);
	}
}
void Block::calcParticles()
{
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
	const double left = bx*world->c_BlockSize;
	const double top = by*world->c_BlockSize;
	const double right = left + world->c_BlockSize;
	const double bottom = top + world->c_BlockSize;
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
	if (lineSegementsIntersect(l1, l2, p1, p2, intersection, 0))
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
	if (lineSegementsIntersect(l1, l2, p2, p3, intersection, 0))
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
	if (lineSegementsIntersect(l1, l2, p3, p4, intersection, 0))
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
	if (lineSegementsIntersect(l1, l2, p4, p1, intersection, 0))
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
bool Block::lineSegementsIntersect(Vector&p, Vector&p2, Vector&q, Vector&q2, Vector&intersection, double precision)
{
	Vector r = p2 - p;
	Vector s = q2 - q;
	double rxs = r.cross(s);
	if (rxs > 1e-5 || -rxs > 1e-5)
	{
		Vector p_q = q - p;
		double t = p_q.cross(s) / rxs;
		double u = p_q.cross(r) / rxs;
		if ((0 + precision < t && t < 1 - precision) && (0 + precision < u && u < 1 - precision))
		{
			intersection = p + (r*t);
			return true;
		}
	}
	return false;
}

double Block::getTemperature()const
{
	return temperature;
}
double Block::getPresure()const
{
	return world->c_FlowConstant * totalParticles * temperature / (world->c_BlockSize*world->c_BlockSize); 
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
