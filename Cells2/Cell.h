#pragma once

class Cell;

#include "WorldSettings.h"
#include "CellFrame.h"
#include "DNA.h"
#include "Membrane.h"

class Cell : public CellFrame{
public:
	Cell(shared_ptr<DNA> tDna, WorldSettings*_ws, const Vector& tCenter, const double tRadius);
	~Cell();

	shared_ptr<DNA> getDNA();

	Membrane* getOuterMembrane()const;

	vector<Cell*> split();

private:
	shared_ptr<DNA> dna;

	Membrane* outerMembrane;
};


