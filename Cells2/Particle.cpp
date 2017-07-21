#include "Particle.h"

Particle::Particle(int _id, float _weight, bool _composed, int _base1, int _base2)
{
	id = _id;
	mass = _weight;
	composed = _composed;
	base1 = _base1;
	base2 = _base2;
}
