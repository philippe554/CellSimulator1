#pragma once

#include "WorldSettings.h"

class Mixture
{
protected:
	float energy;
	float particles[WorldSettings::e_AmountOfParticles];
};