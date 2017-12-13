#pragma once

class Particle;

class Particle
{
public:
	void init(float _unitMass, bool _composed, int _base1, int _base2, float _distributionByMass);

	float unitMass;
	bool composed;
	int base1;
	int base2;

	float normalDistribution;
};

