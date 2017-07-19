#pragma once
#include <vector>

class Reactor;

#include "DNA.h";

using namespace std;

class Reactor
{
public:
	Reactor(float _volume);

	static void loadPrototypes();
	static void printPrototypes();

	void incubate();
	void exchange(Reactor*other,bool membrame, double surface);

	bool compose(int i,int amount);
	bool decompose(int i,int amount);

	static const int amountOfProteins = 50;
	static const int amountOfHeavyProteins = 50;
private:
	vector<DNA> dnas;
	int elements[25];
	int proteins[amountOfProteins+amountOfHeavyProteins];

	static bool prototypesLoaded;
	static int primes[25];
	static vector<int> prototype[amountOfProteins + amountOfHeavyProteins];

	float temperature;
	float volume;
};
