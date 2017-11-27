#include "Cell.h"

Cell::Cell(shared_ptr<DNA> tDna, WorldSettings*_ws, const Vector& tCenter, const double tRadius)
	: CellFrame(_ws, tCenter,tRadius)
{
	age = 0;
	dna = tDna;
	outerMembrane = new Membrane(dna->membrane);
}

Cell::~Cell()
{
	delete outerMembrane;
}

void Cell::cellLogic()
{
	age++;
	/*if (age % 500 == 0)
	{
		growTail();
	}

	for (int i = 0; i < tailLength; i++)
	{
		float c1 = dna->tail.getElementScaled(i, 0, -0.4, 0.4);
		float c2 = dna->tail.getElementScaled(i, 1, 0, 0.5);
		float c3 = dna->tail.getElementScaled(i, 2, 100, 500);
		float c4 = dna->tail.getElementScaled(i, 3, -1, 1);

		setTailFibers(i, c1*sin(age / c3 + c4), -c1*sin(age / c3 + c4), c2*(sin(age / c3 + c4)+1.0));
	}*/
}

Membrane* Cell::getOuterMembrane() const
{
	return outerMembrane;
}

vector<Cell*> Cell::split()
{
	if (stage == 6)
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