#pragma once
#include <vector>

class Reactor;

#include "World.h"
#include "Membrane.h"
#include "Particle.h"
#include "ParticleFactory.h"

class Reactor
{
public:
	enum particlesName
	{
		p_hydrogen, p_carbon, p_oxygen, p_nitrogen,
		e_AmountOfParticles
	};

	static Particle prototypes[];

	Reactor(World* world, float _volume, float _temperature);

	double getTemperature()const;
	double getPresure()const;
	double getAmountOfParticles()const;
	double getMass()const;
	double getConcentration(const int& particle)const;

	void exchange(Reactor*other, double surface, Membrane* membrane);
	void exchange(Reactor*other, double surface, double flow);

	void applyFactory(ParticleFactory* factory);

private:
	World *world;
	float temperature;
	float volume;
	float particles[e_AmountOfParticles];
};
