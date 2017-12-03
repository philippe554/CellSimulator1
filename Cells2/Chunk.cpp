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

	openCLInitFlag = false;

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

	if (openCLInitFlag)
	{
		delete[] buffers;
	}
}
void Chunk::linkChunk(int x, int y, int i1, int i2)
{
	neighbours[i1] = world->findChunk_C(x, y);
	if (neighbours[i1] != nullptr)
	{
		neighbours[i1]->neighbours[i2] = this;
	}
}

void Chunk::initOpenCL(cl::Device _device)
{
	cl::Context context({ _device });
	cl::Program::Sources sources;

	std::string kernel_code =
		"   __kernel void simple_add("
		"		__global float* px, __global float* py,"
		"		__global float* vx, __global float* vy,"
		"		__global float* ax, __global float* ay,"
		"		__global const float* mass, __global const float* radius,"
		"		const int count"
		"		)"
		"	{"
		"		int id = get_global_id(0);"
		"		float2 pos = (float2)(px[id],py[id]);"
		"		float2 vel = (float2)(vx[id],vy[id]);"
		"		float2 acc = (float2)(ax[id],ay[id]);"
		"		float rad = radius[id];"
		"		for(int i=id+1; i<count; i++)"
		"		{"
		"			float2 posOther = (float2)(px[i],py[i]);"
		"			float2 line = posOther - pos;"
		"			float radSum = rad + radius[i];"
		"			if(line.x * line.x + line.y * line.y < radSum * radSum)"
		"			{"
		"				float len = length(line);"
		"				float2 force = (line * radSum / len) - line;"
		"				acc = acc + force * -0.6f;"
		"				ax[i] = ax[i] + force.x * 0.6f;"
		"				ay[i] = ay[i] + force.y * 0.6f;"
		"				float2 velOther = (float2)(vx[i],vy[i]);"
		"				float2 velLine = velOther - vel;"
		"				acc = acc + velLine * 0.01f;"
		"				ax[i] = ax[i] + velLine.x * -0.01f;"
		"				ay[i] = ay[i] + velLine.y * -0.01f;"
		"			}"
		"		}"
		"		ax[id] = acc.x;"
		"		ay[id] = acc.y;"
		"   } "
		"  ";

	std::string kernel_code2 = "\n" \
		"#pragma OPENCL EXTENSION cl_khr_fp64 : enable                    \n" \
		"__kernel void vecAdd(  __global double *a,                       \n" \
		"                       __global double *b,                       \n" \
		"                       __global double *c,                       \n" \
		"                       const unsigned int n)                    \n" \
		"{                                                               \n" \
		"    //Get our global thread ID                                  \n" \
		"    int id = get_global_id(0);                                  \n" \
		"                                                                \n" \
		"    //Make sure we do not go out of bounds                      \n" \
		"    if (id < n)                                                 \n" \
		"        c[id] = a[id] + b[id];                                  \n" \
		"}                                                               \n" \
		"\n";

	sources.push_back({ kernel_code.c_str(),kernel_code.length() });

	cl::Program program(context, sources);
	if (program.build({ _device }) != CL_SUCCESS)
	{
		throw "OpenCLError";
	}

	queue = cl::CommandQueue(context, _device);

	kernel = cl::Kernel(program, "simple_add");
	buffers = new cl::Buffer[9];
	for (int i = 0; i < 8; i++)
	{
		buffers[i] = cl::Buffer(context, CL_MEM_READ_WRITE, sizeof(float) * world->ws.maxParticlesPerChunk);
		kernel.setArg(i, buffers[i]);
	}
	buffers[8] = cl::Buffer(context, CL_MEM_READ_WRITE, sizeof(int));
	kernel.setArg(8, buffers[8]);

	openCLInitFlag = true;
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
		if (world->ws.openCLOptimalization)
		{
			openCLTest();
		}
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

void Chunk::openCLTest()
{
	throw "Old Code";

	float* px = new float[world->ws.maxParticlesPerChunk];
	float* py = new float[world->ws.maxParticlesPerChunk];
	float* vx = new float[world->ws.maxParticlesPerChunk];
	float* vy = new float[world->ws.maxParticlesPerChunk];
	float* ax = new float[world->ws.maxParticlesPerChunk];
	float* ay = new float[world->ws.maxParticlesPerChunk];
	float* mass = new float[world->ws.maxParticlesPerChunk];
	float* radius = new float[world->ws.maxParticlesPerChunk];

	int counter = 0;
	const int size = world->ws.chunkSize*world->ws.chunkSize;
	for (int i = 0; i < size; ++i)
	{
		for (int j = 0; j < blocks[i]->getAmountOfPoints(); j++)
		{
			px[counter] = blocks[i]->getPoint(j)->getPlace().getX();
			py[counter] = blocks[i]->getPoint(j)->getPlace().getY();
			vx[counter] = blocks[i]->getPoint(j)->getVelocity().getX();
			vy[counter] = blocks[i]->getPoint(j)->getVelocity().getY();
			ax[counter] = 0.0f;
			ay[counter] = 0.0f;
			mass[counter] = blocks[i]->getPoint(j)->getMass();
			radius[counter] = blocks[i]->getPoint(j)->getRadius();
			counter++;
			if (counter > world->ws.maxParticlesPerChunk)
			{
				throw "gpu memory allowcation too small";
			}
		}
	}

	queue.enqueueWriteBuffer(buffers[0], CL_TRUE, 0, sizeof(float) * world->ws.maxParticlesPerChunk, px);
	queue.enqueueWriteBuffer(buffers[1], CL_TRUE, 0, sizeof(float) * world->ws.maxParticlesPerChunk, py);
	queue.enqueueWriteBuffer(buffers[2], CL_TRUE, 0, sizeof(float) * world->ws.maxParticlesPerChunk, vx);
	queue.enqueueWriteBuffer(buffers[3], CL_TRUE, 0, sizeof(float) * world->ws.maxParticlesPerChunk, vy);
	queue.enqueueWriteBuffer(buffers[4], CL_TRUE, 0, sizeof(float) * world->ws.maxParticlesPerChunk, ax);
	queue.enqueueWriteBuffer(buffers[5], CL_TRUE, 0, sizeof(float) * world->ws.maxParticlesPerChunk, ay);
	queue.enqueueWriteBuffer(buffers[6], CL_TRUE, 0, sizeof(float) * world->ws.maxParticlesPerChunk, mass);
	queue.enqueueWriteBuffer(buffers[7], CL_TRUE, 0, sizeof(float) * world->ws.maxParticlesPerChunk, radius);
	queue.enqueueWriteBuffer(buffers[8], CL_TRUE, 0, sizeof(int), &counter);

	queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(counter), cl::NullRange);
	queue.finish();

	queue.enqueueReadBuffer(buffers[4], CL_TRUE, 0, sizeof(float) * world->ws.maxParticlesPerChunk, ax);
	queue.enqueueReadBuffer(buffers[5], CL_TRUE, 0, sizeof(float) * world->ws.maxParticlesPerChunk, ay);
	queue.finish();

	counter = 0;
	for (int i = 0; i < size; ++i)
	{
		for (int j = 0; j < blocks[i]->getAmountOfPoints(); j++)
		{
			//blocks[i]->getPoint(j)->addForce(ax[counter], ay[counter]);
			counter++;
		}
	}
	delete[] px;
	delete[] py;
	delete[] vx;
	delete[] vy;
	delete[] ax;
	delete[] ay;
	delete[] mass;
	delete[] radius;
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

