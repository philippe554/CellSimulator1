#include "Cell.h"

Cell::Cell(shared_ptr<DNA> tDna, WorldSettings*_ws, const Vector& tCenter, const double tRadius)
	: CellFrame(_ws,tCenter,tRadius)
{
	dna = tDna;
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

vector<Cell*> Cell::split()
{
	if (stage == 5)
	{
		Cell* cell1 = new Cell(dna->mutate(0.1), ws, center.getPlace(), unit);
		Cell* cell2 = new Cell(dna->mutate(0.1), ws, splitPoints[0].getPlace(), unit);

		splitFrame(cell1, cell2);

		return { cell1, cell2 };
	}
	else
	{
		return vector<Cell*>();
	}
}

shared_ptr<DNA> Cell::getDNA()
{
	dna->fitness = center.getPlace().getX();
	return dna;
}