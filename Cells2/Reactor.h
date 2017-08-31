#pragma once
#include <mutex>

class Reactor;

#include "Mixture.h"
#include "WorldSettings.h"
#include "Membrane.h"
#include "Particle.h"


class Reactor : public Mixture
{
public:
	static Particle prototypes[];

	Reactor();
	virtual ~Reactor() = default;

	void init(WorldSettings* _ws, float _volume, float _temperature);

	float getTemperature()const;
	float getPressure()const;
	float getAmountOfParticles()const;
	float getMass()const;
	float getConcentration(const int particle)const;
	virtual float calcVolume()const =0;
	float getVolume()const;
	float getEnergy()const;
	float getParticle(const int particle)const;
	float getFlowIndex(const int index)const;

	void applyForce(const int index, const float amount);

	void calcExchange(Reactor*other, const int movingCache, const float surface, Membrane* membrane);
	void calcExchange(Reactor*other, const int movingCache, const float surface, const int flowIndex);

	void exchange(Reactor* other, const int movingCache, const float surface);

	void cacheParameters();

protected:
	WorldSettings* ws;

	//basic variables
	float particlesMoving[2*WorldSettings::e_AmountOfParticles];

	//cache
	float temperature;
	float pressure;
	float mass;
	float amountOfParticles;
	float volume;

	std::mutex exchangeMutex;

	float flow[2];
};
