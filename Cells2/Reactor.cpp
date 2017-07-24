#include "Reactor.h"

Particle Reactor::prototypes[WorldSettings::e_AmountOfParticles] = {
	Particle(WorldSettings::p_hydrogen,2,false,0,0),
	Particle(WorldSettings::p_carbon,10,false,0,0),
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
	float energyEntering = 0;

	float amountOfParticles = getAmountOfParticles();
	float amountOfParticlesOther = other->getAmountOfParticles();

	float energy = amountOfParticles * temperature;
	float energyOther = amountOfParticlesOther * other->temperature;

	float pressureDif = getPressure() - other->getPressure();

	for (int i = 0; i < WorldSettings::e_AmountOfParticles; i++)
	{
		float movingPump;
		if(membrane->ports[i]>0)
		{
			movingPump = ws->membranePumpStrength * membrane->ports[i] * getConcentration(i);
		}
		else
		{
			movingPump = ws->membranePumpStrength * membrane->ports[i] * other->getConcentration(i);
		}
		float movingPressure;
		if(pressureDif>0)
		{
			movingPressure = surface * ws->membraneResistance * pressureDif * getConcentration(i);
		}
		else
		{
			movingPressure = surface * ws->membraneResistance * pressureDif * other->getConcentration(i);
		}
		float moving = movingPump + movingPressure;
		if(moving>0)
		{
			energyLeaving += moving * temperature;
		}
		else
		{
			energyEntering += -moving * other->temperature;
		}
		particles[i] -= moving;
		other->particles[i] += moving;
		if(particles[i]<0 || other->particles[i]<0)
		{
			//throw "ERROR!";
		}
	}
	other->temperature = (energyOther + energyLeaving - energyEntering) / other->getAmountOfParticles();
	temperature = (energy - energyLeaving + energyEntering) / getAmountOfParticles();
}

void Reactor::exchange(Reactor* other, const float surface, double flow)
{
	float energyLeaving = 0;
	float energyEntering = 0;

	float amountOfParticles = getAmountOfParticles();
	float amountOfParticlesOther = other->getAmountOfParticles();

	float energy = amountOfParticles * temperature;
	float energyOther = amountOfParticlesOther * other->temperature;

	float pressureDif = getPressure() - other->getPressure();

	for (int i = 0; i < WorldSettings::e_AmountOfParticles; i++)
	{
		float moving;
		if (pressureDif>0)
		{
			moving = surface * ws->flowConstant * pressureDif * getConcentration(i);
		}
		else
		{
			moving = surface * ws->flowConstant * pressureDif * other->getConcentration(i);
		}
		if (moving>0)
		{
			energyLeaving += moving * temperature;
		}
		else
		{
			energyEntering += -moving * other->temperature;
		}
		particles[i] -= moving;
		other->particles[i] += moving;
		if (particles[i]<0 || other->particles[i]<0)
		{
			//throw "ERROR!";
		}
	}
	other->temperature = (energyOther + energyLeaving - energyEntering) / other->getAmountOfParticles();
	temperature = (energy - energyLeaving + energyEntering) / getAmountOfParticles();
}


