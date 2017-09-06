#include "Chunk.h"
#include <iostream>

Chunk::Chunk(World*_world,const int _cx,const int _cy, long long _time)
{
	runningMutex.lock();
	running = true;
	runningMutex.unlock();
	time = _time;
	world = _world;
	cx = _cx;
	cy = _cy;
	blocks = new Block*[world->ws.chunkSize*world->ws.chunkSize];

	for (int i = 0; i < world->ws.chunkSize; i++)
	{
		for (int j = 0; j < world->ws.chunkSize; j++)
		{
			blocks[i * world->ws.chunkSize + j] = nullptr;
		}
	}

	for(int i=0;i<world->ws.chunkSize;i++)
	{
		for(int j=0;j<world->ws.chunkSize;j++)
		{
			blocks[i*world->ws.chunkSize + j] = new Block(world, this, cx, cy, cx*world->ws.chunkSize + i, cy*world->ws.chunkSize + j);
		}
	}

	linkChunk(cx + 1, cy, 0, 4);
	linkChunk(cx + 1, cy + 1, 1, 5);
	linkChunk(cx, cy + 1, 2, 6);
	linkChunk(cx - 1, cy + 1, 3, 7);

	linkChunk(cx - 1, cy, 4, 0);
	linkChunk(cx - 1, cy - 1, 5, 1);
	linkChunk(cx, cy - 1, 6, 2);
	linkChunk(cx + 1, cy - 1, 7, 3);

	runningMutex.lock();
	running = false;
	runningMutex.unlock();
}
Chunk::~Chunk()
{
	runningMutex.lock();
	running = true;
	runningMutex.unlock();
	for(int i=0;i<world->ws.chunkSize*world->ws.chunkSize;i++)
	{
		delete blocks[i];
	}
	delete[] blocks;
}
void Chunk::linkChunk(int x, int y, int i1, int i2)
{
	neighbours[i1] = world->findChunk_C(x, y);
	if (neighbours[i1] != nullptr)
	{
		neighbours[i1]->neighbours[i2] = this;
	}
}

Block* Chunk::findBlock_B(int x, int y) const
{	
	if (x < cx*world->ws.chunkSize || x >= (cx + 1)*world->ws.chunkSize || y < cy*world->ws.chunkSize || y >= (cy + 1)*world->ws.chunkSize)
	{
		return nullptr;
	}
	if (x >= 0) 
	{
		x = x%world->ws.chunkSize;
	}
	else
	{
		x = (world->ws.chunkSize - ((-x) % world->ws.chunkSize)) % world->ws.chunkSize;
	}
	if (y >= 0) 
	{
		y = y%world->ws.chunkSize;
	}
	else
	{
		y = (world->ws.chunkSize - ((-y) % world->ws.chunkSize)) % world->ws.chunkSize;
	}
	return blocks[x*world->ws.chunkSize+y];
}
Block* Chunk::findBlock_N(const int input) const
{
	if(input>=0&&input<world->ws.chunkSize*world->ws.chunkSize)
	{
		return blocks[input];
	}
	else
	{
		return nullptr;
	}
}

void Chunk::run()
{
	acceptAllCells();
	const int size = world->ws.chunkSize*world->ws.chunkSize;
	for (int i = 0; i < size; ++i)
	{
		blocks[i]->stage1();
	}
	for (int i = 0; i < size; ++i)
	{
		blocks[i]->stage2();
	}
	for (int i = 0; i < size; ++i)
	{
		blocks[i]->stage3();
	}
	for (int i = 0; i < size; ++i)
	{
		blocks[i]->stage4();
	}

	time++;
	runningMutex.lock();
	running = false;
	runningMutex.unlock();
}

void Chunk::acceptAllCells()
{
	acceptedCellsMutex.lock();
	while (acceptedCells.size()>0)
	{
		Block*block = findBlock_B(world->calcBlock(acceptedCells.at(0)->getCenter().getX()),
			world->calcBlock(acceptedCells.at(0)->getCenter().getY()));
		if (block != nullptr)
		{
			block->giveCell(acceptedCells.at(0));
		}
		else
		{
			//TODO: Cell is added to wrong chunk
			throw "";
		}
		acceptedCells.erase(acceptedCells.begin());
	}
	acceptedCellsMutex.unlock();
}

vector<shared_ptr<DNA>> Chunk::getDNA()
{
	vector<shared_ptr<DNA>> data;
	for (int i = 0; i < world->ws.chunkSize*world->ws.chunkSize; i++)
	{
		vector<shared_ptr<DNA>> toBeAdded = blocks[i]->getDNA();
		data.insert(data.end(), toBeAdded.begin(), toBeAdded.end());
	}
	return data;
}

void Chunk::giveCell(Cell* cell)
{
	acceptedCellsMutex.lock();
	acceptedCells.push_back(cell);
	acceptedCellsMutex.unlock();
}

long long Chunk::getTime() const
{
	return time;
}
long long Chunk::getMaxTimeDifference()const
{
	long long max = 0;
	for (int i = 0; i<8; i++)
	{
		if (neighbours[i] != nullptr)
		{
			if (time - neighbours[i]->getTime() > max)
			{
				max = time - neighbours[i]->getTime();
			}
		}
	}
	return max;
}
void Chunk::schedule()
{
	runningMutex.lock();
	running = true;
	runningMutex.unlock();
}
bool Chunk::isRunning()const
{
	for (int i = 0; i < 8; ++i)
	{
		if (neighbours[i] != nullptr)
		{
			if (neighbours[i]->running)
			{
				return true;
			}
		}
	}

	return running;
}

void Chunk::addLine(const double x1, const double y1, const double x2, const double y2)
{
	const double left = cx*world->ws.blockSize * world->ws.chunkSize;
	const double top = cy*world->ws.blockSize * world->ws.chunkSize;
	const double right = left + world->ws.blockSize * world->ws.chunkSize;
	const double bottom = top + world->ws.blockSize * world->ws.chunkSize;
	Vector p1(left, top);
	Vector p2(right, top);
	Vector p3(right, bottom);
	Vector p4(left, bottom);
	Vector l1(x1, y1);
	Vector l2(x2, y2);
	bool intersectionFound = false;
	if ((left < x1&&x1 <= right&&top < y1&&y1 <= bottom) || (left < x2&&x2 <= right&&top < y2&&y2 <= bottom))
	{
		intersectionFound = true;
	}
	Vector intersection(0.0, 0.0);
	if (Shapes::lineSegementsIntersect(l1, l2, p1, p2, intersection, 0))
	{
		if (l1.getY() < l2.getY())
		{
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
		if (l1.getX() > l2.getX())
		{
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
		if (l1.getY() > l2.getY())
		{
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
		if (l1.getX() < l2.getX())
		{
			l1 = intersection;
		}
		else
		{
			l2 = intersection;
		}
		intersectionFound = true;
	}
	if (intersectionFound)
	{
		for (int i = 0; i < world->ws.chunkSize*world->ws.chunkSize; i++)
		{
			blocks[i]->addLine(l1.getX(), l1.getY(), l2.getX(), l2.getY());
		}
	}
}

