#pragma once
#include "Vector.h"

class Reactor;

#include "WorldSettings.h"
#include "Membrane.h"
#include "Particle.h"

class Reactor
{
public:
	static Particle prototypes[];

	Reactor(WorldSettings* _ws, float _volume);

	float getTemperature()const;
	float getPressure()const;
	float getAmountOfParticles()const;
	float getMass()const;
	float getConcentration(const int& particle)const;
	float getVolume()const;
	float getParticle(const int& particle)const;

protected:
	void exchange(Reactor*other, const float surface, Membrane* membrane);
	void exchange(Reactor*other, const float surface, double flow);

	WorldSettings* ws;
	float temperature;
	float volume;
	float particles[WorldSettings::e_AmountOfParticles];
};
