#pragma once
#include "Vector.h"

class Reactor;

#include "WorldSettings.h"
#include "Membrane.h"
#include "Particle.h"

class Reactor
{
public:
	virtual ~Reactor() = default;
	static Particle prototypes[];

	Reactor(WorldSettings* _ws, float _temperature);

	void setDefaultParticles(float volume);
	void setTemperature(float _temperature);

	float getTemperature()const;
	float getPressure()const;
	float getAmountOfParticles()const;
	float getMass()const;
	float getConcentration(const int& particle)const;
	virtual float getVolume()const =0;
	float getParticle(const int& particle)const;

protected:
	void exchange(Reactor*other, const float surface, Membrane* membrane);
	void exchange(Reactor*other, const float surface, double flow);

	WorldSettings* ws;

private:
	float temperature;
	float particles[WorldSettings::e_AmountOfParticles];
};
