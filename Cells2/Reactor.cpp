#include "Reactor.h"

Particle Reactor::prototypes[WorldSettings::e_AmountOfParticles] = {
	Particle(WorldSettings::p_hydrogen,2,false,0,0),
	Particle(WorldSettings::p_carbon,10,false,0,0),
	Particle(WorldSettings::p_oxygen,22,false,0,0),
	Particle(WorldSettings::p_nitrogen,24,false,0,0)
};

Reactor::Reactor(WorldSettings* _ws)
{
	ws = _ws;
}

void Reactor::init(float _volume, float _temperature)
{
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

float Reactor::getConcentration(const int& particle) const
{
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

void Reactor::calcExchange(Reactor* other, const int movingCache, const float surface, double flow)
{
	const float pressureDif = getPressure() - other->getPressure();
	const float sfp = surface * ws->flowConstant * pressureDif;
	const int index = movingCache*WorldSettings::e_AmountOfParticles;

	for (int i = 0; i < WorldSettings::e_AmountOfParticles; i++)
	{
		if (pressureDif>0)
		{
			particlesMoving[index + i] = sfp * getConcentration(i);
		}
		else
		{
			particlesMoving[index + i] = sfp * other->getConcentration(i);
		}
	}
}

void Reactor::exchange(Reactor* other, const int movingCache)
{
	float energyLeaving = 0;
	float energyEntering = 0;

	const int index = movingCache*WorldSettings::e_AmountOfParticles;

	for (int i = 0; i < WorldSettings::e_AmountOfParticles; i++)
	{
		float moving = particlesMoving[index + i];
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
	}
	other->energy += (energyLeaving - energyEntering);
	energy += (-energyLeaving + energyEntering);
}

void Reactor::cacheParameters()
{
	volume = getVolume();

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


/*int flowCounter = 0;
Vector flowSum(0.0, 0.0);
float presure = getPressure();
for (int j = 0; j < 8; j+=2)
{
if (neighbours[j] != nullptr)
{
flowCounter++;
double flowRate = (neighbours[j]->getPressure() - presure)*world->c_FlowRate;
Vector flowSub((bx - neighbours[j]->bx)*flowRate, (by - neighbours[j]->by)*flowRate);
flowSum += flowSub;
}
}
if (neighbours[6] != nullptr)
{
if(getTemperature()>neighbours[6]->getTemperature())
{
flowSum.add(0, (neighbours[6]->getTemperature() - getTemperature())*0.1);
}
}
if(flowCounter>0)
{
//flow = (flow*0.25)+(flowSum * (0.75 * world->ws.blockSize / flowCounter));
}
else
{
//flow.multiply(0.25);
}
//flow.add(frictionForce / getMass());
frictionForce.set(0, 0);*/


