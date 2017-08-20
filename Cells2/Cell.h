#pragma once

class Cell;

#include "World.h"
#include "CellFrame.h"
#include "Vector.h"
#include "Point.h"
#include "Joint.h"
#include "Line.h"
#include "DNA.h"
#include "Membrane.h"

class Cell : public CellFrame, public Reactor, public Shapes {
public:
	Cell(shared_ptr<DNA> tDna, World*tWorld, Vector tCenter, double tRadius);
	~Cell();

	shared_ptr<DNA> getDNA();

	bool connectCells(Cell*other);
	void disconnectCells(int );

	void applyPressure(float p);
	double getSurface()const;
	float getRadius()const;
	float getVolume()const override;

	Membrane* getOuterMembrane()const;

	int getId()const;
private:
	shared_ptr<DNA> dna;

	Membrane* outerMembrane;

	Cell*connectedCells[AmountOfEdges];
};


