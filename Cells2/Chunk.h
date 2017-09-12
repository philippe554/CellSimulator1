#pragma once

#include <mutex>
#include <memory>

#include "WorldSettings.h"

#include <CL/cl2.hpp>

class Chunk;

#include "World.h"
#include "Block.h"
#include "DNA.h"
#include "Cell.h"

using namespace std;

class Chunk
{
public:
	Chunk(World*tWorld,const int tcx, const int tcy, long long _time);
	~Chunk();

	void initOpenCL(cl::Device _device);

	Block* findBlock_B(int x,  int y)const;
	Block* findBlock_N(const int input)const;

	void run();

	long long getTime()const;
	long long getMaxTimeDifference()const;

	void giveCell(Cell*cell);
	void acceptAllCells();
	vector<shared_ptr<DNA>> getDNA();

	void schedule();
	bool isRunning()const;

	void addLine(const double x1, const double y1, const double x2, const double y2);

	World* world;
private:
	void linkChunk(int x, int y, int i1, int i2);

	int cx;
	int cy;

	Chunk* neighbours[8];
	Block** blocks;

	vector<Cell*> acceptedCells;
	mutex acceptedCellsMutex;

	bool running;
	mutex runningMutex;

	bool openCLInitFlag;
	cl::CommandQueue queue;
	cl::Kernel* kernels;
	cl::Buffer* buffers;

	long long time;
};
