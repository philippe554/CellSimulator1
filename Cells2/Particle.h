#pragma once

class Particle
{
public:
	Particle(int _id, float _weight, bool _composed, int _base1, int _base2);

	int id;
	float mass;
	bool composed;
	int base1;
	int base2;
};

