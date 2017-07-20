#include "World.h"
#include "Core.h"

World::World()
{
	time = 0;
	Core::setup(c_Cores);
	for(int i=0;i<c_WorldBoundary;i++)
	{
		for(int j=0;j<c_WorldBoundary;j++)
		{
			chunks[make_pair(i, j)] = new Chunk(this, i, j,time);
		}
	}
}

World::~World()
{
	for(auto chunk : chunks)
	{
		delete chunk.second;
		chunk.second = nullptr;
	}
}

void World::jump(long long amount, bool visualReady)
{
	const long long target = time + amount;
	bool allFinished = false;
	while (!allFinished)
	{
		allFinished = true;
		for(auto chunk:chunks)
		{
			if(chunk.second->getTime() < target)
			{
				allFinished = false;
				if (!chunk.second->isRunning()) 
				{
					if (chunk.second->getMaxTimeDifference() == 0)
					{
						chunk.second->schedule();
						Core::runChunk(chunk.second);
					}
				}
			}
		}
	}
	while (!Core::allFinished()){}
	if(visualReady)
	{
		for (auto chunk : chunks)
		{
			chunk.second->acceptAllCells();
		}
	}
	time = target;
}

void World::addCell(shared_ptr<DNA> dna, const double x,const double y)
{
	Block* block = findBlock_B(calcBlock(x), calcBlock(y));

	if (block != nullptr)
	{
		Cell* newCell = new Cell(dna, this, Vector(x, y), c_NewCellRadius);
		block->cells.push_back(newCell);

#ifdef S_CellAutoConnect
		for (auto cell : block->cells)
		{
			if (cell != newCell)
			{
				cell->connectCells(newCell);
				cell->connectCells(newCell);
			}
		}
		for (auto neighbour : block->neighbours)
		{
			if (neighbour != nullptr)
			{
				for (auto cell : neighbour->cells)
				{
					if (cell != newCell)
					{
						cell->connectCells(newCell);
						cell->connectCells(newCell);
					}
				}
			}
		}
#endif
	}
}

Chunk* World::findChunk_C(const int x,const int y)
{
	if (chunks.count(make_pair(x, y)) > 0)
	{
		return chunks.at(make_pair(x, y));
	}
	return nullptr;
}
Block* World::findBlock_B(const int x, const int y)
{
	Chunk* chunk = findChunk_C(calcChunk_B(x), calcChunk_B(y));
	if (chunk != nullptr) {
		return chunk->findBlock_B(x, y);
	}
	return nullptr;
}
int World::calcChunk(const double input)const
{
	const double length = c_BlockSize*c_ChunkSize;
	return int((input >= 0 ? input : input - length + 1) / length);
}
int World::calcBlock(const double input) const
{
	return int((input >= 0 ? input : input - c_BlockSize + 1) / c_BlockSize);
}
int World::calcChunk_B(const int input)
{
	return int(double(input >= 0 ? input : input - c_ChunkSize + 1) / c_ChunkSize);
}

vector<shared_ptr<DNA>> World::getDNA()
{
	vector<shared_ptr<DNA>> data;
	for (auto chunk : chunks)
	{
		vector<shared_ptr<DNA>> toBeAdded = chunk.second->getDNA();
		data.insert(data.end(), toBeAdded.begin(), toBeAdded.end());
	}
	return data;
}

long long World::getTime()
{
	return time;
}

/*void World::step()
{
#ifdef timers
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	std::chrono::steady_clock::time_point end;
#endif
	const double precision = 1.0 / c_JointPrecision;
	map<long, shared_ptr<Point>>::iterator i = allPoints.begin();
	while (i != allPoints.end())
	{
		const auto& point = (*i).second;
		if (!point->isRefed()) {
			point->backupLocation();
			point->addForce(Vector(0.0, c_Gravity*point->getMass()));
			i++;
		}
		else
		{
			i = allPoints.erase(i);
		}
	}
#ifdef timers
	end = std::chrono::steady_clock::now();
	stats_ForceCalcPreTime += (end - begin);
	begin = std::chrono::steady_clock::now();
#endif
	for (int j = 0; j < c_JointPrecision; j++)
	{
		for (const auto& point : allPoints)
		{
			point.second->calcForcesJoints();
		}
		for (const auto& point : allPoints)
		{
			//try {
			point.second->applyForces(precision);
			//}catch(std::exception const & ex)
			//{
			//	string s = ex.what();
			//}
		}
	}
#ifdef timers
	end = std::chrono::steady_clock::now();
	stats_ForceCalcTime += (end - begin);
	begin = std::chrono::steady_clock::now();
#endif
	for (const auto& point : allPoints)
	{
		point.second->clearForces();
	}
#ifdef timers
	end = std::chrono::steady_clock::now();
	stats_ForceCalcPostTime += (end - begin);
	begin = std::chrono::steady_clock::now();
#endif
	for (auto& chunk : chunks)
	{
		chunk.second->step();
	}
#ifdef timers
	end = std::chrono::steady_clock::now();
	stats_CollisionTime += (end - begin);
	begin = std::chrono::steady_clock::now();
#endif
	for (auto& chunk : chunks)
	{
		chunk.second->calcFlow();
		chunk.second->calcTemp();
		chunk.second->doRestructure();
	}
#ifdef timers
	end = std::chrono::steady_clock::now();
	stats_RestructureTime += (end - begin);
#endif
}*/
