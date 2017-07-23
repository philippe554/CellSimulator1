#pragma once

#include "Membrane.h"

Membrane::Membrane(const Matrix& _ports)
{
	for(int i=0;i<WorldSettings::e_AmountOfParticles;i++)
	{
		ports[i] = _ports.getElement(i, 0);
	}
}
