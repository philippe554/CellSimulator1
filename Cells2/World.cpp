#include "World.h"
#include "Core.h"

World::World(WorldSettings _ws)
{
	time = 0;
	ws = _ws;
	Core::setup(ws.c_Cores);
	for(int i=0;i<ws.c_WorldBoundary;i++)
	{
		for(int j=0;j<ws.c_WorldBoundary;j++)
		{
			chunks[make_pair(i, j)] = new Chunk(this, i, j,time);

			float product = (ws.c_WorldBoundary * ws.blockSize * ws.chunkSize) - 2;

			/*chunks[make_pair(i, j)]->addLine(1, 1, 1, product);
			chunks[make_pair(i, j)]->addLine(1, 1, product, 1);
			chunks[make_pair(i, j)]->addLine(1, product, product, product);
			chunks[make_pair(i, j)]->addLine(product, 1, product, product);*/
		}
	}

	for (int i = 0; i < ws.c_WorldBoundary; i++)
	{
		for (int j = 0; j < ws.chunkSize; j+=3)
		{
			chunks[make_pair(i, 0)]->addPoint(i * ws.blockSize * ws.chunkSize + j * ws.blockSize, 2, 100);
			chunks[make_pair(i, ws.c_WorldBoundary-1)]->addPoint(i * ws.blockSize * ws.chunkSize + j * ws.blockSize, 
				ws.c_WorldBoundary * ws.blockSize * ws.chunkSize -2, 100);

			chunks[make_pair(0, i)]->addPoint(2, i * ws.blockSize * ws.chunkSize + j * ws.blockSize, 100);
			chunks[make_pair(ws.c_WorldBoundary - 1, i)]->addPoint(ws.c_WorldBoundary * ws.blockSize * ws.chunkSize - 2,
				i * ws.blockSize * ws.chunkSize + j * ws.blockSize, 100);
		}
	}
	
	if (ws.openCLOptimalization)
	{
		std::vector<cl::Platform> all_platforms;
		cl::Platform::get(&all_platforms);
		if (all_platforms.size() == 0) throw "OpenCL Error";
		cl::Platform default_platform = all_platforms[0];
		
		std::vector<cl::Device> all_devices;
		default_platform.getDevices(CL_DEVICE_TYPE_ALL, &all_devices);
		if (all_devices.size() == 0) throw "OpenCL Error";
		cl::Device default_device = all_devices[0];

		for (auto chunk : chunks)
		{
			chunk.second->initOpenCL(default_device);
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
	const long long target = time + (amount*2);
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
						chunk.second->setAsScheduled();
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
			chunk.second->acceptObjects();
		}
	}
	time = target;
}

void World::addCell(shared_ptr<DNA> dna, const double x,const double y)
{
	Block* block = findBlock_B(calcBlock(x), calcBlock(y));

	if (block != nullptr)
	{
		block->createCell(dna, Vector(x, y));
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
	const double length = ws.blockSize*ws.chunkSize;
	return int((input >= 0 ? input : input - length + 1) / length);
}
int World::calcBlock(const double input) const
{
	return int((input >= 0 ? input : input - ws.blockSize + 1) / ws.blockSize);
}
int World::calcChunk_B(const int input)
{
	return int(double(input >= 0 ? input : input - ws.chunkSize + 1) / ws.chunkSize);
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
	return time/2;
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
