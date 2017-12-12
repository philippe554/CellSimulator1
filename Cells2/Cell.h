#pragma once

class Cell;

#include "WorldSettings.h"
#include "CellFrame.h"
#include "DNA.h"
#include "Membrane.h"

class Cell : public CellFrame{
public:
	Cell(shared_ptr<DNA> tDna, WorldSettings*_ws, const Vector& tCenter);
	Cell(Cell * parent);
	~Cell();

	void cellLogic();

	shared_ptr<DNA> getDNA();

	Membrane* getOuterMembrane()const;

	bool readyToSplit();
	Cell* split();

private:
	shared_ptr<DNA> dna;

	Membrane* outerMembrane;

	long age;
};


