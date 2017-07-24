#pragma once

class WorldSettings
{
public: 
	WorldSettings();

	float blockSize = 10;
	int chunkSize = 10;
	
	float defaultTemperature = 100;
	float surfacePressure = 1;
	float flowConstant = 0.01;
	float membraneResistance = 0.1;
	float membranePumpStrength = 0.5;

	enum particlesName
	{
		p_hydrogen, p_carbon, p_oxygen, p_nitrogen,
		e_AmountOfParticles
	};
};