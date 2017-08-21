#include "Cell.h"

Cell::Cell(shared_ptr<DNA> tDna, WorldSettings*_ws, const Vector& tCenter, const double tRadius)
	: CellFrame(_ws,tCenter,tRadius)
{
	outerMembrane = new Membrane(dna->membrane);
}

Cell::~Cell()
{
	delete outerMembrane;
}

Membrane* Cell::getOuterMembrane() const
{
	return outerMembrane;
}

shared_ptr<DNA> Cell::getDNA()
{
	dna->fitness = center.getPlace().getX();
	return dna;
}