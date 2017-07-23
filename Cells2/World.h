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
	World();
	~World();

	void jump(long long amount,bool visualReady);

	void addCell(shared_ptr<DNA> dna, const double x, const double y);

	Chunk* findChunk_C(const int x, const int y);
	Block* findBlock_B(const int x, const int y);

	int calcChunk(const double input) const;
	int calcBlock(const double input) const;
	int calcChunk_B(const int input);

	vector<shared_ptr<DNA>> getDNA();

	const static int c_Cores = 3;

	const int c_WorldBoundary = 6;

	const double c_WaterLevel = 0;
	const double c_WaterFriction = 0.00;//2;
	const double c_Gravity = 0.02;

	const double c_CellCellCollisionForce = 2;// 0.54;//0.3;
	const double c_CellCellFriction = 1;// 0.4;
	const double c_LineCellCollisionForce = 0.99;// 0.3;
	const double c_LineCellFriction = 0.4;

	const double c_NewCellRadius = 2;
	const double c_NewCellRadiusStrength = 5.5;// 0.8;
	const double c_NewCellSideStrength = 3.07;// 0.8;
	const double c_NewCellRadiusDamping = 0.5;// 0.8;
	const double c_NewCellSideDamping = 0.3;// 0.8;

	const double c_Precision = 0.2;
	
	const double c_FlowRate = 0.005;
	const double c_DefaultChunkParticleCount = 1000;
	const double c_ChunkRediation = 0.00001;
	const double c_DefaultTemperature = 300;
	const double c_ParticalMass=0.001;
	const double c_FlowConstant = 1.0 / (c_DefaultChunkParticleCount*c_DefaultTemperature);
	const double c_TermalFlowConstant = 0.1;
	const double c_CellHeatProduction = 100;

//#define S_CellCellCollision
//#define S_CellAutoConnect

	long stats_CellsCreated=0;
	long stats_CellsBroken=0;
	long stats_PointsCreated=0;
	long stats_PointsDestroyed=0;

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


