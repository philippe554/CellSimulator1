#pragma once

class WorldSettings
{
public: 
	WorldSettings();

	//General
	float blockSize = 10;
	int chunkSize = 10;

	//Cells
	float maxExpantion = 5;

	//Flow Engine
	float defaultTemperature = 100;
	float surfacePressure = 0.001;
	float flowConstant = 0.001;
	float membraneResistance = 0.01;
	float membranePumpStrength = 0.2;

	enum particlesName
	{
		p_hydrogen, p_carbon, p_oxygen, p_nitrogen,
		e_AmountOfParticles
	};
};
