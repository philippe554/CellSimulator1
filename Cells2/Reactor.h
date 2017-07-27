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

	Reactor(WorldSettings* _ws);

	void init(float _volume, float _temperature);

	float getTemperature()const;
	float getPressure()const;
	float getAmountOfParticles()const;
	float getMass()const;
	float getConcentration(const int& particle)const;
	virtual float getVolume()const =0;
	float getParticle(const int& particle)const;

	void calcExchange(Reactor*other, const int movingCache, const float surface, Membrane* membrane);
	void calcExchange(Reactor*other, const int movingCache, const float surface, double flow);

	void exchange(Reactor* other, const int movingCache);

	void cacheParameters();

protected:
	WorldSettings* ws;

private:
	//basic variables
	float energy;
	float particles[WorldSettings::e_AmountOfParticles];
	float particlesMoving[2*WorldSettings::e_AmountOfParticles];

	//cache
	float temperature;
	float pressure;
	float mass;
	float amountOfParticles;
	float volume;
};
