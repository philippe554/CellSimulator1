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
	blocks = new Block*[World::c_ChunkSize*World::c_ChunkSize];

	for (int i = 0; i < World::c_ChunkSize; i++)
	{
		for (int j = 0; j < World::c_ChunkSize; j++)
		{
			blocks[i*World::c_ChunkSize + j] = nullptr;
		}
	}

	for(int i=0;i<World::c_ChunkSize;i++)
	{
		for(int j=0;j<World::c_ChunkSize;j++)
		{
			blocks[i*World::c_ChunkSize + j] = new Block(world, this, cx, cy, cx*World::c_ChunkSize + i, cy*World::c_ChunkSize + j);
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
	for(int i=0;i<World::c_ChunkSize*World::c_ChunkSize;i++)
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
	if (x < cx*world->c_ChunkSize || x >= (cx + 1)*world->c_ChunkSize || y < cy*world->c_ChunkSize || y >= (cy + 1)*world->c_ChunkSize)
	{
		return nullptr;
	}
	if (x >= 0) 
	{
		x = x%world->c_ChunkSize;
	}
	else
	{
		x = (world->c_ChunkSize - ((-x) % world->c_ChunkSize)) % world->c_ChunkSize;
	}
	if (y >= 0) 
	{
		y = y%world->c_ChunkSize;
	}
	else
	{
		y = (world->c_ChunkSize - ((-y) % world->c_ChunkSize)) % world->c_ChunkSize;
	}
	return blocks[x*world->c_ChunkSize+y];
}
Block* Chunk::findBlock_N(const int input) const
{
	if(input>=0&&input<world->c_ChunkSize*world->c_ChunkSize)
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

	for (int i = 0; i < World::c_ChunkSize*World::c_ChunkSize; i++)
	{
		blocks[i]->calcJointForces();

#ifdef S_CellCellCollision
		blocks[i]->cellCellCollision();
#endif
	}
	for (int i = 0; i < World::c_ChunkSize*World::c_ChunkSize; i++)
	{
		blocks[i]->movePoints(world->c_Precision, world->c_WaterFriction);
	}

	for (int i = 0; i < World::c_ChunkSize*World::c_ChunkSize; i++)
	{
		blocks[i]->calcFlow();
		blocks[i]->moveFlow();
		blocks[i]->doRestructure();
	}

	time++;
	//world->coutMutex.lock();
	//cout << "run chunk (" << cx << ";" << cy << "): " << time << endl;
	//world->coutMutex.unlock();
	runningMutex.lock();
	running = false;
	runningMutex.unlock();
}

void Chunk::acceptAllCells()
{
	acceptedCellsMutex.lock();
	while (acceptedCells.size()>0)
	{
		Block*block = findBlock_B(world->calcBlock(acceptedCells.at(0)->getCenter()->getX()),
			world->calcBlock(acceptedCells.at(0)->getCenter()->getY()));
		if (block != nullptr)
		{
			block->cells.push_back(acceptedCells.at(0));
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
	for (int i = 0; i < World::c_ChunkSize*World::c_ChunkSize; i++)
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
	return running;
}

