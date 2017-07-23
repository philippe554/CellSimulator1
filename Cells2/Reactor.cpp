#include "Reactor.h"

Particle Reactor::prototypes[WorldSettings::e_AmountOfParticles] = {
	Particle(WorldSettings::p_hydrogen,2,false,0,0),
	Particle(WorldSettings::p_carbon,20,false,0,0),
	Particle(WorldSettings::p_oxygen,22,false,0,0),
	Particle(WorldSettings::p_nitrogen,24,false,0,0)
};

Reactor::Reactor(WorldSettings* _ws, float _temperature)
{
	ws = _ws;
	temperature = _temperature;
}

void Reactor::setDefaultParticles(float volume)
{
	particles[WorldSettings::p_hydrogen] = 0.5 * volume;
	particles[WorldSettings::p_carbon] = 0.1 * volume;
	particles[WorldSettings::p_oxygen] = 0.02 * volume;
	particles[WorldSettings::p_nitrogen] = 0.005 * volume;
}

void Reactor::setTemperature(float _temperature)
{
	temperature = _temperature;
}

float Reactor::getTemperature()const
{
	return temperature;
}
float Reactor::getPressure()const
{
	return getAmountOfParticles() * temperature / getVolume();
}

float Reactor::getAmountOfParticles() const
{
	float total = 0;
	for (int i = 0; i< WorldSettings::e_AmountOfParticles; i++)
	{
		total += particles[i];
	}
	return total;
}
float Reactor::getMass() const
{
	float total = 0;
	for (int i = 0; i<WorldSettings::e_AmountOfParticles; i++)
	{
		total += particles[i] * prototypes[i].mass;
	}
	return total;
}

float Reactor::getConcentration(const int& particle) const
{
	float amountOfParticles = getAmountOfParticles();
	if (amountOfParticles > 0)
	{
		return particles[particle] / amountOfParticles;
	}
	return 0.0;
}

float Reactor::getParticle(const int& particle) const
{
	return particles[particle];
}

void Reactor::exchange(Reactor* other, const float surface, Membrane* membrane)
{
	float energyLeaving = 0;
	float amountOfParticlesOther = other->getAmountOfParticles();
	float energyOther = amountOfParticlesOther * other->temperature;

	for (int i = 0; i < WorldSettings::e_AmountOfParticles; i++)
	{
		if(membrane->ports[i]>0)
		{
			//float concentrationRatio = getConcentration(i) / other->getConcentration(i);
			//float moving = particles[i] * concentrationRatio * membrane->ports[i] * surface / amountOfParticles;
			float moving = 0.0001 * membrane->ports[i] * getConcentration(i) * surface;
			energyLeaving += moving * temperature;
			particles[i] -= moving;
			other->particles[i] += moving;
		}		
	}
	other->temperature = (energyLeaving + energyOther) / other->getAmountOfParticles();

	float energyEntering = 0;
	float amountOfParticles = getAmountOfParticles();
	float energy = amountOfParticles * temperature;

	for (int i = 0; i < WorldSettings::e_AmountOfParticles; i++)
	{
		if (membrane->ports[i]<0)
		{
			//float concentrationRatio = other->getConcentration(i) / getConcentration(i);
			//float moving = other->particles[i] * concentrationRatio * -membrane->ports[i] * surface / amountOfParticlesOther;
			float moving = 0.0001 * -membrane->ports[i] * other->getConcentration(i) * surface;
			energyEntering += moving * temperature;
			particles[i] += moving;
			other->particles[i] -= moving;
		}
	}
	temperature = (energyEntering + energy) / getAmountOfParticles();
}

void Reactor::exchange(Reactor* other, const float surface, double flow)
{
	float amount = surface * flow;
	float energy = getAmountOfParticles() * temperature;
	float energyNeighbour = other->getAmountOfParticles() * other->temperature;

	if (amount > 0)
	{
		float amountOfParticles = getAmountOfParticles();
		for (int i = 0; i < WorldSettings::e_AmountOfParticles; i++)
		{
			float moving = (particles[i] * amount) / amountOfParticles;
			particles[i] -= moving;
			other->particles[i] += moving;
		}
		other->temperature = (amount*temperature + energyNeighbour) / other->getAmountOfParticles();
	}
	else
	{
		float amountOfParticles = other->getAmountOfParticles();
		for (int i = 0; i < WorldSettings::e_AmountOfParticles; i++)
		{
			float moving = (other->particles[i] * amount) / amountOfParticles;
			particles[i] -= moving;
			other->particles[i] += moving;
		}
		temperature = (amount*other->temperature + energy) / getAmountOfParticles();
	}
}


