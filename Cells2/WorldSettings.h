#pragma once

#include <math.h>

class WorldSettings;

#include "Particle.h"

class WorldSettings
{
public: 
	WorldSettings();

	//General
	float blockSize = 3;
	int chunkSize = 10;
	float precision = 0.2;
	bool openCLOptimalization = false;
	int maxParticlesPerChunk = 100;

	//Cells
	float maxExpantion = 5;

	//Flow Engine
	float defaultTemperature = 100;
	float temperatureFlowConstant = 0.001;
	float surfacePressure = 0.001;
	float flowConstant = 0.01;
	float flowBuildUp = 0.8;
	float flowBuildUpCorrected = pow(flowBuildUp, precision);
	float membraneResistance = 0.01;
	float membranePumpStrength = 0.2;

	//Particles
	enum particlesName
	{
		p_hydrogen, p_carbon, p_oxygen, p_nitrogen,
		e_AmountOfParticles
	};
	Particle particleProtoType[e_AmountOfParticles];
	void setupMixure(float* arr, float _mass);



	///



	int c_Cores = 3;

	int c_WorldBoundary = 3;

	double c_WaterLevel = 0;
	double c_WaterFriction = 0.00;//2;
	double c_Gravity = 0.0;//0.02;

	double c_CellCellCollisionForce = 2;// 0.54;//0.3;
	double c_CellCellFriction = 1;// 0.4;
	double c_LineCellCollisionForce = 0.99;// 0.3;
	double c_LineCellFriction = 0.4;

	double c_NewCellRadius = 2;
	double c_NewCellRadiusStrength = 0.3;//5.5;// 0.8;
	double c_NewCellSideStrength = 0.1;// 3.07;// 0.8;
	double c_NewCellRadiusDamping = 0.5;// 0.8;
	double c_NewCellSideDamping = 0.3;// 0.8;

	double c_Precision = 0.2;

	double c_FlowRate = 0.005;
	double c_DefaultChunkParticleCount = 1000;
	double c_ChunkRediation = 0.00001;
	double c_DefaultTemperature = 300;
	double c_ParticalMass = 0.001;
	double c_FlowConstant = 1.0 / (c_DefaultChunkParticleCount*c_DefaultTemperature);
	double c_TermalFlowConstant = 0.1;
	double c_CellHeatProduction = 100;

	//#define S_CellCellCollision
	//#define S_CellAutoConnect

	long stats_CellsCreated = 0;
	long stats_CellsBroken = 0;
	long stats_PointsCreated = 0;
	long stats_PointsDestroyed = 0;
};
