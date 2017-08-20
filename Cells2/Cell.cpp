#include "Cell.h"

Cell::Cell(shared_ptr<DNA> tDna, World * tWorld, Vector tCenter, double tRadius)
{
	outerMembrane = new Membrane(dna->membrane);
	init(Cell::getVolume(), tWorld->ws.defaultTemperature);

	for (int i = 0; i < AmountOfEdges; i++)
	{
		connectedCells[i] = nullptr;
	}
}

Membrane* Cell::getOuterMembrane() const
{
	return outerMembrane;
}
void Cell::applyPressure(float p)
{
	for (auto joint : edgeJoints)
	{
		Vector jointLine = Vector(joint->p1->getPlace(), joint->p2->getPlace());
		float surface = jointLine.getLength();
		Vector normal = jointLine.getPerpendicularCounterClockwise().getUnit();
		normal.multiply(0.5*surface*p);
		joint->p1->addForce(normal);
		joint->p2->addForce(normal);
	}
}

float Cell::getSurface()const
{
	return 2 * radius * 3.1415926535897;
}

float Cell::getVolume() const
{
	float total = 0;

	for (int i = 0; i<AmountOfEdges - 1; i++)
	{
		total += surfaceTriangle(Vector::getLength(center->getPlace(), edgePoints[i]->getPlace()),
			Vector::getLength(center->getPlace(), edgePoints[i + 1]->getPlace()),
			Vector::getLength(edgePoints[i]->getPlace(), edgePoints[i + 1]->getPlace()));
	}
	total += surfaceTriangle(Vector::getLength(center->getPlace(), edgePoints[0]->getPlace()),
		Vector::getLength(center->getPlace(), edgePoints[AmountOfEdges - 1]->getPlace()),
		Vector::getLength(edgePoints[0]->getPlace(), edgePoints[AmountOfEdges - 1]->getPlace()));

	return total;
}

bool Cell::connectCells(Cell*other)
{
	int finalIndexOwn = 0;
	int finalIndexOther = 0;
	float distance = 10000;
	for (int i = 0; i<AmountOfEdges; i++)
	{
		for (int j = 0; j<AmountOfEdges; j++)
		{
			float d = pow(Vector::getLength(edgePoints[i]->getPlace(), other->edgePoints[j]->getPlace()), 2);
			if (d<distance && connectedCells[i] == nullptr &&  other->connectedCells[j] == nullptr)
			{
				finalIndexOwn = i;
				finalIndexOther = j;
				distance = d;
			}
		}
	}
	if (distance<2)
	{
		connectedCells[finalIndexOwn] = other;
		other->connectedCells[finalIndexOther] = this;

		if (edgePoints[finalIndexOwn]->getReal()->getID() == other->edgePoints[finalIndexOther]->getReal()->getID())
		{
			return false;
		}
		shared_ptr<Point> p = Point::MakePoint(edgePoints[finalIndexOwn], other->edgePoints[finalIndexOther]);
		edgePoints[finalIndexOwn]->setRef(p);
		other->edgePoints[finalIndexOther]->setRef(p);
		edgePoints[finalIndexOwn] = p;
		other->edgePoints[finalIndexOther] = p;

		return true;
	}
	else {
		return false;
	}
}
void Cell::disconnectCells(int i)
{
	Cell*other = connectedCells[i];

	if (connectedCells[i] != nullptr) {
		edgePoints[i] = edgePoints[i]->getSubPoint(center->getMass(), id, other->id);
		connectedCells[i] = nullptr;

		for (int l = 0; l < AmountOfEdges; l++)
		{
			if (other->connectedCells[l] == this)
			{
				other->connectedCells[l] = nullptr;
			}
		}
	}
}

shared_ptr<DNA> Cell::getDNA()
{
	dna->fitness = center->getPlace()->getX();
	return dna;
}