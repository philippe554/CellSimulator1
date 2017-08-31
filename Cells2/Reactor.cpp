#include "Reactor.h"

Particle Reactor::prototypes[WorldSettings::e_AmountOfParticles] = {
	Particle(WorldSettings::p_hydrogen,2,false,0,0),
	Particle(WorldSettings::p_carbon,10,false,0,0),
	Particle(WorldSettings::p_oxygen,22,false,0,0),
	Particle(WorldSettings::p_nitrogen,24,false,0,0)
};

Reactor::Reactor()
{
}

void Reactor::init(WorldSettings* _ws, float _volume, float _temperature)
{
	ws = _ws;
	for (auto f : flow)
	{
		f = 0;
	}

	volume = _volume;

	particles[WorldSettings::p_hydrogen] = 0.5 * volume;
	particles[WorldSettings::p_carbon] = 0.1 * volume;
	particles[WorldSettings::p_oxygen] = 0.02 * volume;
	particles[WorldSettings::p_nitrogen] = 0.005 * volume;

	float sum = 0;
	for (int i = 0; i < WorldSettings::e_AmountOfParticles; i++)
	{
		sum += particles[i];
	}
	energy = sum * _temperature;

	cacheParameters();
}

float Reactor::getTemperature()const
{
	return temperature;
}
float Reactor::getPressure()const
{
	return pressure;
}
float Reactor::getAmountOfParticles() const
{
	return amountOfParticles;
}
float Reactor::getMass() const
{
	return mass;
}

float Reactor::getConcentration(const int particle) const
{
	if (amountOfParticles > 0)
	{
		return particles[particle] / amountOfParticles;
	}
	return 0.0;
}

float Reactor::getVolume() const
{
	return volume;
}

float Reactor::getEnergy() const
{
	return energy;
}

float Reactor::getParticle(const int particle) const
{
	return particles[particle];
}

float Reactor::getFlowIndex(const int index) const
{
	return flow[index];
}

void Reactor::applyForce(const int index, const float amount)
{
	flow[index] += amount / getMass();
}

void Reactor::calcExchange(Reactor* other, const int movingCache, const float surface, Membrane* membrane)
{
	const float pressureDif = getPressure() - other->getPressure();
	const float smp = surface * ws->membraneResistance * pressureDif;
	const int index = movingCache*WorldSettings::e_AmountOfParticles;

	for (int i = 0; i < WorldSettings::e_AmountOfParticles; i++)
	{
		if(membrane->ports[i]>0)
		{
			particlesMoving[index + i] = ws->membranePumpStrength * membrane->ports[i] * getConcentration(i);
		}
		else
		{
			particlesMoving[index + i] = ws->membranePumpStrength * membrane->ports[i] * other->getConcentration(i);
		}
		if(pressureDif>0)
		{
			particlesMoving[index + i] += smp * getConcentration(i);
		}
		else
		{
			particlesMoving[index + i] += smp * other->getConcentration(i);
		}
	}
}

void Reactor::calcExchange(Reactor* other, const int movingCache, const float surface, const int flowIndex)
{
	const float pressureDif = (getPressure() - other->getPressure()) * ws->flowConstant;
	flow[flowIndex] = ws->flowBuildUpCorrected * flow[flowIndex] + (1.0-ws->flowBuildUpCorrected) * pressureDif;
	const float sfp = surface * flow[flowIndex];
	const int index = movingCache*WorldSettings::e_AmountOfParticles;

	for (int i = 0; i < WorldSettings::e_AmountOfParticles; i++)
	{
		if (flow[flowIndex]>0)
		{
			particlesMoving[index + i] = sfp * getConcentration(i);
		}
		else
		{
			particlesMoving[index + i] = sfp * other->getConcentration(i);
		}
	}
}

void Reactor::exchange(Reactor* other, const int movingCache, const float surface)
{
	exchangeMutex.lock();

	float temperatureDif = temperature - other->getTemperature();
	float tcs = temperatureDif * ws->temperatureFlowConstant * surface * ws->precision;

	float energyLeaving = tcs;
	float energyEntering = -tcs;

	const int index = movingCache * WorldSettings::e_AmountOfParticles;

	for (int i = 0; i < WorldSettings::e_AmountOfParticles; i++)
	{
		float moving = particlesMoving[index + i] * ws->precision;
		if (moving>0)
		{
			energyLeaving += moving * temperature;
		}
		else
		{
			energyEntering += -moving * other->getTemperature();
		}
		particles[i] -= moving;
		other->particles[i] += moving;
	}
	other->energy += (energyLeaving - energyEntering);
	energy += (-energyLeaving + energyEntering);

	exchangeMutex.unlock();
}

void Reactor::cacheParameters()
{
	volume = calcVolume();

	amountOfParticles = 0;
	mass = 0;
	for (int i = 0; i< WorldSettings::e_AmountOfParticles; i++)
	{
		if (particles[i] < 0)
		{
			throw "ERROR!";
		}
		amountOfParticles += particles[i];
		mass += particles[i] * prototypes[i].mass;
	}

	if(amountOfParticles <= 0)
	{
		temperature = 0;
	}
	else
	{
		temperature = energy / amountOfParticles;
	}
	if(volume <= 0)
	{
		throw "Error";
	}
	pressure = energy / volume;
}

