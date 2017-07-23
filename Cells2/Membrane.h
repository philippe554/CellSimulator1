#pragma once

#include "Matrix.h"
#include "WorldSettings.h"

class Membrane
{
public:
	Membrane(const Matrix& _ports);

	float ports[WorldSettings::e_AmountOfParticles];
};