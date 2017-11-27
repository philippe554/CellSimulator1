#include "WorldSettings.h"

WorldSettings::WorldSettings()
{
	particleProtoType[p_hydrogen].init(1.0, false, 0, 0, 80);
	particleProtoType[p_carbon].init(10.0, false, 0, 0, 5);
	particleProtoType[p_oxygen].init(12.0, false, 0, 0, 10);
	particleProtoType[p_nitrogen].init(14.0, false, 0, 0, 5);

	float sum = 0;
	for (int i = 0; i < WorldSettings::e_AmountOfParticles; i++)
	{
		sum += particleProtoType[i].normalDistribution;
	}
	for (int i = 0; i < WorldSettings::e_AmountOfParticles; i++)
	{
		particleProtoType[i].normalDistribution /= sum;
	}
}

void WorldSettings::setupMixure(float * arr, float _mass)
{
	for (int i = 0; i < WorldSettings::e_AmountOfParticles; i++)
	{
		arr[i] = _mass * particleProtoType[i].normalDistribution / particleProtoType[i].unitMass;
	}
}
