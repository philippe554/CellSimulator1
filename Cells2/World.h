#pragma once
#include <chrono>
#include <map>

class World;

#define timers

#include "WorldSettings.h"
#include "Block.h"
#include "Chunk.h"

using namespace std;

class World
{
public:
	World(WorldSettings _ws);
	~World();

	void jump(long long amount,bool visualReady);

	void addCell(shared_ptr<DNA> dna, const double x, const double y);

	Chunk* findChunk_C(const int x, const int y);
	Block* findBlock_B(const int x, const int y);

	int calcChunk(const double input) const;
	int calcBlock(const double input) const;
	int calcChunk_B(const int input);

	vector<shared_ptr<DNA>> getDNA();

	map<pair<int, int>, Chunk*> chunks;

	long long getTime();

	mutex coutMutex;

	WorldSettings ws;

private:
	long long time;

#ifdef timers
	std::chrono::steady_clock::duration stats_RestructureTime;
	std::chrono::steady_clock::duration stats_CollisionTime;
	std::chrono::steady_clock::duration stats_ForceCalcPreTime;
	std::chrono::steady_clock::duration stats_ForceCalcTime;
	std::chrono::steady_clock::duration stats_ForceCalcPostTime;

#endif
};


