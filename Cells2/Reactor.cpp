#include "Reactor.h"

Particle Reactor::prototypes[e_AmountOfParticles] = {
	Particle(p_hydrogen,1,false,0,0),
	Particle(p_carbon,8,false,0,0),
	Particle(p_oxygen,9,false,0,0),
	Particle(p_nitrogen,10,false,0,0)
};

Reactor::Reactor(World* _world, float _volume, float _temperature)
{
	world = _world;
	volume = _volume;
	temperature = _temperature;
}

double Reactor::getTemperature()const
{
	return temperature;
}
double Reactor::getPresure()const
{
	return world->c_FlowConstant * getAmountOfParticles() * temperature / volume;
}
double Reactor::getAmountOfParticles() const
{
	float total = 0;
	for (int i = 0; i<e_AmountOfParticles; i++)
	{
		total += particles[i];
	}
	return total;
}
double Reactor::getMass() const
{
	float total = 0;
	for (int i = 0; i<e_AmountOfParticles; i++)
	{
		total += particles[i] * prototypes[i].mass;
	}
	return total;
}

double Reactor::getConcentration(const int& particle) const
{
	return particles[particle] / getAmountOfParticles();
}


