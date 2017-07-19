//#define lawEnhancer
#define UI

#ifdef UI
#include <Windows.h> 

#include "App.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int CmdShow)
{
	//Heap error --> close
	HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);

	if (SUCCEEDED(CoInitialize(NULL)))
	{
		App app;
		if (SUCCEEDED(app.Initialize(CmdShow)))
		{
			app.RunMessageLoop();
		}
		CoUninitialize();
	}
}
#endif
#include <memory>
#include <chrono>
#include "Reactor.h"

#ifdef lawEnhancer
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>
using namespace std;
#include "World.h"

bool compare(World *w1,World *w2)
{
	return w1->stats_CellsBroken < w2->stats_CellsBroken;
}

void testPhysics(World* world, Vector SpawnPoint)
{
	srand(time(NULL));
	for (int i = 0; i<100; i++)
	{
		int r1 = rand() % 30 + 1;
		int r2 = rand() % 30 + 1;
		world->addCell(50 + r1, 55 + r2);
		world->jump(1000, false);
		cout << "Progress: " << i << "%" << endl;
	}
}

double mutateVar(double i1, double i2, int max)
{
	int choose = rand() % 3;
	if(choose==0)
	{
		return i1;
	}else if(choose==1)
	{
		return i2;
	}else
	{
		return (rand() % (max*100)) / 100.0;
	}
}

/*void mutate(vector<World*>& worlds)
{
	for (int i = 0; i < 5; i++)
	{
		if(i<worlds.size())
		{
			World* fresh = new World();
			fresh->c_CellCellCollisionForce = worlds[i]->c_CellCellCollisionForce;
			fresh->c_LineCellCollisionForce = worlds[i]->c_LineCellCollisionForce;
			fresh->c_NewCellRadiusStrength = worlds[i]->c_NewCellRadiusStrength;
			fresh->c_NewCellSideStrength = worlds[i]->c_NewCellSideStrength;
			delete worlds[i];
			worlds[i] = fresh;
		}
	}
	for(int i=5;i<worlds.size();i++)
	{
		delete worlds[i];
		worlds.erase(worlds.begin() + i);
		i--;
	}
	int startSize = worlds.size();
	while(worlds.size()<20)
	{
		World* parent1 = worlds.at(rand() % startSize);
		World* parent2 = worlds.at(rand() % startSize);
		World* child = new World();
		child->c_CellCellCollisionForce = mutateVar(parent1->c_CellCellCollisionForce, parent2->c_CellCellCollisionForce,2);
		child->c_LineCellCollisionForce = mutateVar(parent1->c_LineCellCollisionForce, parent2->c_LineCellCollisionForce,4);
		child->c_NewCellRadiusStrength = mutateVar(parent1->c_NewCellRadiusStrength, parent2->c_NewCellRadiusStrength,6);
		child->c_NewCellSideStrength = mutateVar(parent1->c_NewCellSideStrength, parent2->c_NewCellSideStrength,6);
		worlds.push_back(child);
	}
}*/

class A
{
public:
	shared_ptr<Point> point;
};

int main()
{
	/*srand(time(NULL));
	vector<World*> worlds;
	worlds.push_back(new World());
	mutate(worlds);
	for(int i=0;i<100;i++)
	{
		for(auto world:worlds)
		{
			testPhysics(world, new Vector(50, 50));
		}
		sort(worlds.begin(), worlds.end(), compare);
		for (auto world : worlds)
		{
			cout << world->stats_CellsBroken << " ";
		}
		cout << endl;
		cout << worlds[0]->c_CellCellCollisionForce << " " << worlds[0]->c_LineCellCollisionForce << " " << " "
			<< " " << worlds[0]->c_NewCellRadiusStrength << " " << " " << worlds[0]->c_NewCellSideStrength << " "<<endl;
		mutate(worlds);
	}*/

	/*cout << "Start" << endl;
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	World world;
	testPhysics(&world, new Vector(50, 50));
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	cout << "Done (using " << world.c_Cores << " cores)" << endl;
	cout <<"Cells Broken: "<< world.stats_CellsBroken<<endl;
	std::cout << "Time difference: " << std::chrono::duration_cast<std::chrono::seconds> (end - begin).count() << "s" << std::endl;
	std::cout << "Time difference: " << std::chrono::duration_cast<std::chrono::microseconds> (end - begin).count() << "ms" << std::endl;
	std::cout << "Time difference: " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() <<"ns"<< std::endl;
	*/

	/*int amountOfPrimes = 10;
	int primes[] = { 1,2,3,5,7,11,13,17,19,23};
	const int maxProduct = 1000;
	int combinations = 6;
	int results[maxProduct];

	for(int i=0;i<maxProduct;i++){
		results[i] = 0;
	}

	for(int i1=0;i1<amountOfPrimes;i1++){
		for (int i2 = 0; i2 < amountOfPrimes; i2++) {
			for (int i3 = 0; i3 < amountOfPrimes; i3++) {
				for (int i4 = 0; i4 < amountOfPrimes; i4++) {
					for (int i5 = 0; i5 < amountOfPrimes; i5++) {
						for (int i6 = 0; i6 < amountOfPrimes; i6++) {
							int product = primes[i1] * primes[i2] * primes[i3] * primes[i4] * primes[i5] * primes[i6];
							if (product <= maxProduct) {
								results[product]++;
							}
						}
					}				
				}
			}
		}
	}

	int distribution[100];
	for(int i=0;i<100;i++)
	{
		distribution[i] = 0;
	}
	for (int i = 0; i < 100; i++) {
		distribution[results[i]]++;
	}
	for (int i = 0; i < 100; i++)
	{
		cout << i << ": " << results[i]<<endl;
	}*/

	//cout << "Forces time (post): " << std::chrono::duration_cast<std::chrono::nanoseconds> (world.stats_ForceCalcPreTime).count() << "ns" << endl;
	//cout << "Forces time:        " << std::chrono::duration_cast<std::chrono::nanoseconds> (world.stats_ForceCalcTime).count() << "ns" << endl;
	//cout << "Forces time (pre):  " << std::chrono::duration_cast<std::chrono::nanoseconds> (world.stats_ForceCalcPostTime).count() << "ns" << endl;
	//cout << "Collision time:     " << std::chrono::duration_cast<std::chrono::nanoseconds> (world.stats_CollisionTime).count() << "ns" << endl;
	//cout << "Restructure time:   " << std::chrono::duration_cast<std::chrono::nanoseconds> (world.stats_RestructureTime).count() << "ns" << endl;
	
	Reactor::loadPrototypes();
	Reactor::printPrototypes();
	
	cin.get();
}
#endif