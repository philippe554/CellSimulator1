#include "Particle.h"

void Particle::init(float _unitMass, bool _composed, int _base1, int _base2, float _distributionByMass)
{
	unitMass = _unitMass;
	composed = _composed;
	base1 = _base1;
	base2 = _base2;
	normalDistribution = _distributionByMass / unitMass;
}

