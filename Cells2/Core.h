#pragma once

#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>

class Core;

#include "Chunk.h"

class Core
{
public:
	Core();
	~Core();

	static void setup(int amount);
	static void runChunk(Chunk* chunk);
	static void end();

	static bool allFinished();
private:
	static Chunk* getNextChunk();

	static vector<Core*> cores;
	static vector<Chunk*> tasks;
	static mutex taskMutex;

	void run();

	thread* t;
	static condition_variable cv;
	bool finished;
	bool stop;
	bool waiting;
};
