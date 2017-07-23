#pragma once

class WorldSettings
{
public: 
	float blockSize = 10;
	int chunkSize = 10;
	float liquidFlowRate = 0.000001;
	float defaultTemperature = 100;
	float surfacePressure = 0.02;

	enum particlesName
	{
		p_hydrogen, p_carbon, p_oxygen, p_nitrogen,
		e_AmountOfParticles
	};
};