#include "Reactor.h"

#include <iostream>

int Reactor::primes[25];
vector<int> Reactor::prototype[amountOfProteins + amountOfHeavyProteins];

void Reactor::loadPrototypes()
{
	primes[0] = 2;
	primes[1] = 3;
	primes[2] = 5;
	primes[3] = 7;
	primes[4] = 11;
	primes[5] = 13;
	primes[6] = 17;
	primes[7] = 19;
	primes[8] = 23;
	primes[9] = 29;
	primes[10] = 31;
	primes[11] = 37;
	primes[12] = 41;
	primes[13] = 43;
	primes[14] = 47;
	primes[15] = 53;
	primes[16] = 59;
	primes[17] = 61;
	primes[18] = 67;
	primes[19] = 71;
	primes[20] = 73;
	primes[21] = 79;
	primes[22] = 83;
	primes[23] = 89;
	primes[24] = 97;
	
	prototype[0].push_back(1);
	for(int i=1;i<amountOfProteins+amountOfHeavyProteins;i++)
	{
		for (int j = i; i >= 1; i--)
		{
			if((i+1)%j==0)
			{
				if (j == 1) 
				{
					prototype[i].push_back((i + 1));
					j = 0;
				}
				else
				{
					prototype[i].push_back(j);
					prototype[i].push_back((i + 1) / j);
					j = 0;
				}
			}
		}
	}
}

void Reactor::printPrototypes()
{
	for (int i = 0; i < amountOfProteins + amountOfHeavyProteins; i++)
	{
		cout << (i + 1) << ": ";
		for(int j=0;j<prototype[i].size();j++)
		{
			cout << prototype[i].at(j) << " ";
		}
		cout << endl;
	}
}
