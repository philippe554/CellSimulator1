#pragma once

class WorldSettings
{
public: 
	float blockSize = 50;
	float chunkSize = 10;
	float liquidFlowRate = 0.000001;
	float defaultTemperature = 300;

	enum particlesName
	{
		p_hydrogen, p_carbon, p_oxygen, p_nitrogen,
		e_AmountOfParticles
	};
};