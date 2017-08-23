#include "CellFrame.h"

long CellFrame::idCounter = 0;

CellFrame::CellFrame(WorldSettings * _ws, const Vector& tCenter, const float radius)
	: Reactor(_ws)
{
	ws->stats_CellsCreated++;
	id = CellFrame::idCounter;
	idCounter++;

	float pointMass = 1;//old
	tailLength = 0;
	hasTailEnd = false;
	stage = 0;
	splitLocation = 0;
	unit = radius;
	splitPointsLength = 0;
	splitJointsLength = 0;

	float x = tCenter.getX();
	float y = tCenter.getY();

	center.init(tCenter.getX(), tCenter.getY(), pointMass);

	edgePoints[0].init(x, y - 20 * radius / 20, pointMass);
	edgePoints[1].init(x + 17.32 * radius / 20, y - 10 * radius / 20, pointMass);
	edgePoints[2].init(x + 17.32 * radius / 20, y + 10 * radius / 20, pointMass);
	edgePoints[3].init(x, y + 20 * radius / 20, pointMass);
	edgePoints[4].init(x - 17.32 * radius / 20, y + 10 * radius / 20, pointMass);
	edgePoints[5].init(x - 17.32 * radius / 20, y - 10 * radius / 20, pointMass);

	radiusJoints[0].init(&center, &edgePoints[0], ws->c_NewCellRadiusStrength, ws->c_NewCellRadiusDamping, false);
	radiusJoints[1].init(&center, &edgePoints[1], ws->c_NewCellRadiusStrength, ws->c_NewCellRadiusDamping, false);
	radiusJoints[2].init(&center, &edgePoints[2], ws->c_NewCellRadiusStrength, ws->c_NewCellRadiusDamping, false);
	radiusJoints[3].init(&center, &edgePoints[3], ws->c_NewCellRadiusStrength, ws->c_NewCellRadiusDamping, false);
	radiusJoints[4].init(&center, &edgePoints[4], ws->c_NewCellRadiusStrength, ws->c_NewCellRadiusDamping, false);
	radiusJoints[5].init(&center, &edgePoints[5], ws->c_NewCellRadiusStrength, ws->c_NewCellRadiusDamping, false);

	edgeJoints[0].init(&edgePoints[0], &edgePoints[1], ws->c_NewCellSideStrength, ws->c_NewCellSideDamping, true);
	edgeJoints[1].init(&edgePoints[1], &edgePoints[2], ws->c_NewCellSideStrength, ws->c_NewCellSideDamping, true);
	edgeJoints[2].init(&edgePoints[2], &edgePoints[3], ws->c_NewCellSideStrength, ws->c_NewCellSideDamping, true);
	edgeJoints[3].init(&edgePoints[3], &edgePoints[4], ws->c_NewCellSideStrength, ws->c_NewCellSideDamping, true);
	edgeJoints[4].init(&edgePoints[4], &edgePoints[5], ws->c_NewCellSideStrength, ws->c_NewCellSideDamping, true);
	edgeJoints[5].init(&edgePoints[5], &edgePoints[0], ws->c_NewCellSideStrength, ws->c_NewCellSideDamping, true);

	for (int i = 0; i < AmountOfEdges; i++)
	{
		connectedCellsMaster[i] = false;
		connectedCells[i] = nullptr;
	}

	init(CellFrame::getVolume(), ws->defaultTemperature);

	/*for (int i = 0; i < dna->tail.getAmountOfRows() - 1; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			int index = i * 3 + j;
			tail.push_back(Point::MakePoint(chunk, x - (index * 20 + 40) * radius / 20, y - 10 * radius / 20, pointMass, id));
			tail.push_back(Point::MakePoint(chunk, x - (index * 20 + 40) * radius / 20, y + 10 * radius / 20, pointMass, id));
			if (index > 0)
			{
				tailJoints.push_back(new Joint(tail[tail.size() - 2], tail[tail.size() - 4], dna->tail.getElementScaled(i, 0, 0.1, 5), dna->tail.getElementScaled(i, 1, 0.1, 1), true, 0, id));
				tailJoints.push_back(new Joint(tail[tail.size() - 2], tail[tail.size() - 3], dna->tail.getElementScaled(i, 0, 0.1, 5), dna->tail.getElementScaled(i, 1, 0.1, 1), false, 0, id));
				tailJoints.push_back(new Joint(tail[tail.size() - 4], tail[tail.size() - 1], dna->tail.getElementScaled(i, 0, 0.1, 5), dna->tail.getElementScaled(i, 1, 0.1, 1), false, 0, id));
				tailJoints.push_back(new Joint(tail[tail.size() - 3], tail[tail.size() - 1], dna->tail.getElementScaled(i, 0, 0.1, 5), dna->tail.getElementScaled(i, 1, 0.1, 1), true, 0, id));
			}
			else
			{
				tailJoints.push_back(new Joint(tail[0], edgePoints[5], dna->tail.getElementScaled(i, 0, 0.1, 5), dna->tail.getElementScaled(i, 1, 0.1, 1), true, 0, id));
				tailJoints.push_back(new Joint(tail[0], edgePoints[4], dna->tail.getElementScaled(i, 0, 0.1, 5), dna->tail.getElementScaled(i, 1, 0.1, 1), false, 0, id));
				tailJoints.push_back(new Joint(edgePoints[5], tail[1], dna->tail.getElementScaled(i, 0, 0.1, 5), dna->tail.getElementScaled(i, 1, 0.1, 1), false, 0, id));
				tailJoints.push_back(new Joint(edgePoints[4], tail[1], dna->tail.getElementScaled(i, 0, 0.1, 5), dna->tail.getElementScaled(i, 1, 0.1, 1), true, 0, id));
			}
			tailJoints.push_back(new Joint(tail[tail.size() - 2], tail[tail.size() - 1], dna->tail.getElementScaled(i, 2, 1., 5), dna->tail.getElementScaled(i, 3, 0.1, 1), false, 0, id));
		}
	}
	tail.push_back(Point::MakePoint(chunk, x - (dna->tail.getAmountOfRows() * 3 * 20 + 40) * radius / 20, y, pointMass, id));
	tailJoints.push_back(new Joint(tail.back(), tail[tail.size() - 3], dna->tail.getElementScaled(-1, 0, 0.1, 5), dna->tail.getElementScaled(-1, 1, 0.1, 1), true, 0, id));
	tailJoints.push_back(new Joint(tail[tail.size() - 2], tail.back(), dna->tail.getElementScaled(-1, 0, 0.1, 5), dna->tail.getElementScaled(-1, 1, 0.1, 1), true, 0, id));	
	*/
}

CellFrame::~CellFrame() {
	for (int i = 0; i < AmountOfEdges; i++)
	{
		disconnectCells(i);
	}
}

void CellFrame::growJoints()
{
	for (auto& joint : radiusJoints)
	{
		joint.grow(ws->precision);
	}
	for (auto& joint : edgeJoints)
	{
		joint.grow(ws->precision);
	}
	for (int i = 0; i < tailLength; i++) {
		tailJoints[i * 5 + 0].grow(ws->precision);
		tailJoints[i * 5 + 1].grow(ws->precision);
		tailJoints[i * 5 + 2].grow(ws->precision);
		tailJoints[i * 5 + 3].grow(ws->precision);
		tailJoints[i * 5 + 4].grow(ws->precision);
	}
	if (hasTailEnd) 
	{
		tailEndJoints[0].grow(ws->precision);
		tailEndJoints[1].grow(ws->precision);
	}
	for (int i = 0; i < AmountOfEdges; i++) 
	{
		if (connectedCellsMaster[i])
		{
			connectedCellsJoints[i * 2 + 0].grow(ws->precision);
			connectedCellsJoints[i * 2 + 1].grow(ws->precision);
		}
	}
	for (int i = 0; i < splitJointsLength; i++) {
		splitJoints[i].grow(ws->precision);
	}
}

Vector CellFrame::calcJointForces(const Vector& flow)
{
	center.calcForcesJoints();
	for (auto& point : edgePoints)
	{
		point.calcForcesJoints();
	}
	for (int i = 0; i < splitPointsLength; i++) {
		splitPoints[i].calcForcesJoints();
	}
	//for (auto& point : tailPoints)
	//{
	//	point.calcForcesJoints();
	//}

	Vector frictionTotal(0.0, 0.0);
	for (auto& joint : edgeJoints)
	{
		frictionTotal.add(joint.calcFriction(flow));
	}
	for (int i = 0; i < splitJointsLength; i++) {
		frictionTotal.add(splitJoints[i].calcFriction(flow));
	}
	//for (auto& joint : tailJoints)
	//{
	//	frictionTotal.add(joint.calcFriction(flow));
	//}
	return frictionTotal;

	/*tailCounter++;
	for (int i = 0; i < dna->tail.getAmountOfRows() - 1; i++)
	{
		for (int j = 0; j < 3; j++) {
			int index = i * 3 + j;
			float c1 = dna->tail.getElementScaled(i, 4, -1, 1);
			float c2 = dna->tail.getElementScaled(i, 5, -1, 1);
			float c3 = dna->tail.getElementScaled(i, 6, 20, 200);
			float c4 = dna->tail.getElementScaled(i, 7, -1, 1);

			tailJoints[i * 15 + j * 5 + 0]->setLength(c1*sin(tailCounter / c3 + c4) + tailJoints[i * 15 + j * 5 + 0]->originalLength);
			tailJoints[i * 15 + j * 5 + 1]->setLength(c1*sin(tailCounter / c3 + c4) + tailJoints[i * 15 + j * 5 + 1]->originalLength);
			tailJoints[i * 15 + j * 5 + 2]->setLength(-c1*sin(tailCounter / c3 + c4) + tailJoints[i * 15 + j * 5 + 2]->originalLength);
			tailJoints[i * 15 + j * 5 + 3]->setLength(-c1*sin(tailCounter / c3 + c4) + tailJoints[i * 15 + j * 5 + 3]->originalLength);
			tailJoints[i * 15 + j * 5 + 4]->setLength(c2*sin(tailCounter / c3 + c4) + tailJoints[i * 15 + j * 5 + 4]->originalLength);
		}
	}
	float c1 = dna->tail.getElementScaled(-1, 4, -1, 1);
	float c3 = dna->tail.getElementScaled(-1, 6, 30, 200);
	float c4 = dna->tail.getElementScaled(-1, 7, -1, 1);
	tailJoints[(dna->tail.getAmountOfRows() - 1) * 15]->setLength(c1*sin(tailCounter / c3 + c4) + tailJoints[(dna->tail.getAmountOfRows() - 1) * 15]->originalLength);
	tailJoints[(dna->tail.getAmountOfRows() - 1) * 15 + 1]->setLength(-c1*sin(tailCounter / c3 + c4) + tailJoints[(dna->tail.getAmountOfRows() - 1) * 15 + 1]->originalLength);
	*/
}

void CellFrame::movePoints(float precision, float backgroundFriction)
{
	float massPoint = getMass() / float(1 + AmountOfEdges + splitPointsLength + tailLength*2 + hasTailEnd?1:0);
	center.applyForces(precision, backgroundFriction, massPoint);
	for (auto& point : edgePoints)
	{
		point.applyForces(precision, backgroundFriction, massPoint);
	}
	for (int i = 0; i < splitPointsLength; i++) {
		splitPoints[i].applyForces(precision, backgroundFriction, massPoint);
	}
	/*for (auto& point : tailPoints)
	{
		point.applyForces(precision, backgroundFriction, massPoint);
	}*/
	if (hasTailEnd) 
	{
		tailEndPoint.applyForces(precision, backgroundFriction, massPoint);
	}
}

void CellFrame::cellCellCollision(CellFrame* other)
{
	cellCellCollisionHelper(other);
	other->cellCellCollisionHelper(this);
}

void CellFrame::cellCellCollisionHelper(CellFrame * other)
{
	for (int i = 0; i < CellFrame::AmountOfEdges; i++)
	{
		for (int j = 0; j < CellFrame::AmountOfEdges; j++)
		{
			Vector intersection(0.0, 0.0);
			if (Shapes::lineSegementsIntersect(center.getPlace(), edgePoints[i].getPlace(),
				other->edgeJoints[j].getP1()->getPlace(), other->edgeJoints[j].getP2()->getPlace(), intersection))
			{
				Vector overlap(intersection, edgePoints[i].getPlace());
				Vector force(other->center.getPlace(), edgePoints[i].getPlace());
				force.makeUnit();
				force.multiply(overlap.getLength() * ws->c_CellCellCollisionForce * edgePoints[i].getMass());
				edgePoints[i].addForce(force);

				float l1 = Vector::getLength(intersection, other->edgeJoints[j].getP1()->getPlace());
				float l2 = Vector::getLength(intersection, other->edgeJoints[j].getP2()->getPlace());

				other->edgeJoints[j].getP1()->addForce(force*((-l2) / (l1 + l2)));
				other->edgeJoints[j].getP2()->addForce(force*((-l1) / (l1 + l2)));
			}
		}
	}
	//friction
	/*Vector otherVel = Vector::getAverage(other->edgeJoints[j]->p1->getVelocity(), other->edgeJoints[j]->p2->getVelocity());
	Vector velDif(otherVel, edgePoints[i]->getVelocity());

	velDif.multiply(world->c_CellCellFriction * edgePoints[i]->getMass() * overlap.getLength());

	other->edgeJoints[j]->p1->addForce(velDif*((l2) / (l1 + l2)));
	other->edgeJoints[j]->p2->addForce(velDif*((l1) / (l1 + l2)));

	float l1Own = Vector::getLength(intersection, center->getPlace());
	float l2Own = Vector::getLength(intersection, edgePoints[i]->getPlace());

	center->addForce       (velDif*((-l2Own) / (l1Own + l2Own)));
	edgePoints[i]->addForce(velDif*((-l1Own) / (l1Own + l2Own)));*/
}

bool CellFrame::connectCells(CellFrame * otherCell)
{
	int own = 0;
	int other = 0;
	float distance = 10000;
	for (int i = 0; i<AmountOfEdges; i++)
	{
		for (int j = 0; j<AmountOfEdges; j++)
		{
			float d = pow(Vector::getLength(edgePoints[i].getPlace(), otherCell->edgePoints[j].getPlace()), 2);
			if (d<distance && connectedCells[i] == nullptr &&  otherCell->connectedCells[j] == nullptr)
			{
				own = i;
				other = j;
				distance = d;
			}
		}
	}
	if (distance<2)
	{
		int ownNext = own + 1 == AmountOfEdges ? 0 : own + 1;
		int otherNext = other + 1 == AmountOfEdges ? 0 : other + 1;

		connectedCells[own] = otherCell;
		otherCell->connectedCells[other] = this;

		connectedCellsMaster[own] = true;
		otherCell->connectedCellsMaster[other] = false;
		
		connectedCellsJoints[own * 2].init(&edgePoints[own], &otherCell->edgePoints[otherNext], ws->c_NewCellRadiusStrength, ws->c_NewCellRadiusDamping, false);
		connectedCellsJoints[own * 2 + 1].init(&edgePoints[ownNext], &otherCell->edgePoints[other], ws->c_NewCellRadiusStrength, ws->c_NewCellRadiusDamping, false);

		connectedCellsJoints[own * 2].setTarget(0, 0.1);
		connectedCellsJoints[own * 2 + 1].setTarget(0, 0.1);

		return true;
	}
	else {
		return false;
	}
}

void CellFrame::disconnectCells(const int i)
{
	CellFrame*other = connectedCells[i];

	if (other != nullptr)
	{
		for (int l = 0; l < AmountOfEdges; l++)
		{
			if (other->connectedCells[l] != nullptr) 
			{
				if (other->connectedCells[l]->getId() == id)
				{
					if (other->connectedCellsMaster[l]) 
					{
						other->connectedCellsJoints[l * 2].deconstruct();
						other->connectedCellsJoints[l * 2 + 1].deconstruct();
					}
					other->connectedCellsMaster[l] = false;
					other->connectedCells[l] = nullptr;
				}
			}
		}
		if (connectedCellsMaster[i]) 
		{
			connectedCellsJoints[i * 2].deconstruct();
			connectedCellsJoints[i * 2 + 1].deconstruct();
		}
		connectedCellsMaster[i] = false;
		connectedCells[i] = nullptr;
	}
}

void CellFrame::startSplit(const int location)
{
	if (stage == 0) 
	{
		splitLocation = location;
		stage = 1;
	}
}

bool CellFrame::nextStage()
{
	if (stage == 1) 
	{
		//horizontal Stretch
		splitJoints[0].init(&edgePoints[(4 + splitLocation) % AmountOfEdges], &edgePoints[(0 + splitLocation) % AmountOfEdges], ws->c_NewCellRadiusStrength, ws->c_NewCellRadiusDamping, false);
		splitJoints[1].init(&edgePoints[(3 + splitLocation) % AmountOfEdges], &edgePoints[(1 + splitLocation) % AmountOfEdges], ws->c_NewCellRadiusStrength, ws->c_NewCellRadiusDamping, false);
		splitJoints[0].setTarget(2.5 * unit, 0.01);
		splitJoints[1].setTarget(2.5 * unit, 0.01);

		splitPointsLength = 0;
		splitJointsLength = 2;
		stage = 2;
		return false;
	}
	else if (stage == 2)
	{
		//split center
		splitPoints[0].init(center.getPlace().getX(), center.getPlace().getY(), 0.0);
		splitPoints[0].setVelocity(center.getVelocity());

		splitJoints[2].init(&edgePoints[(5 + splitLocation) % AmountOfEdges], &splitPoints[0], ws->c_NewCellRadiusStrength, ws->c_NewCellRadiusDamping, false);
		splitJoints[2].setTarget(unit, 0.01);
		splitJoints[3].init(&edgePoints[(2 + splitLocation) % AmountOfEdges], &splitPoints[0], ws->c_NewCellRadiusStrength, ws->c_NewCellRadiusDamping, false);
		splitJoints[3].setTarget(unit, 0.01);

		radiusJoints[(0 + splitLocation) % AmountOfEdges].deconstruct();
		radiusJoints[(1 + splitLocation) % AmountOfEdges].deconstruct();
		radiusJoints[(0 + splitLocation) % AmountOfEdges].init(&splitPoints[0], &edgePoints[(0 + splitLocation) % AmountOfEdges], ws->c_NewCellRadiusStrength, ws->c_NewCellRadiusDamping, false);
		radiusJoints[(0 + splitLocation) % AmountOfEdges].setTarget(unit, 0.01);
		radiusJoints[(1 + splitLocation) % AmountOfEdges].init(&splitPoints[0], &edgePoints[(1 + splitLocation) % AmountOfEdges], ws->c_NewCellRadiusStrength, ws->c_NewCellRadiusDamping, false);
		radiusJoints[(1 + splitLocation) % AmountOfEdges].setTarget(unit, 0.01);

		splitPointsLength = 1;
		splitJointsLength = 4;
		stage = 3;
		return false;
	}
	else if (stage == 3)
	{
		splitJoints[4].init(&center, &splitPoints[0], ws->c_NewCellRadiusStrength, ws->c_NewCellRadiusDamping, false);
		splitJoints[4].setTarget(unit, 0.01);

		splitPointsLength = 1;
		splitJointsLength = 5;
		stage = 4;
		return false;
	}
	else if (stage == 4)
	{
		Vector avgPlace = Vector::getAverage(center.getPlace(),splitPoints[0].getPlace());
		Vector avgVelocity = Vector::getAverage(center.getVelocity(), splitPoints[0].getVelocity());
		splitPoints[1].init(avgPlace.getX(), avgPlace.getY(), 0.0);
		splitPoints[1].setVelocity(avgVelocity);
		splitPoints[2].init(avgPlace.getX(), avgPlace.getY(), 0.0);
		splitPoints[2].setVelocity(avgVelocity);

		splitJoints[5].init(&splitPoints[1], &center, ws->c_NewCellRadiusStrength, ws->c_NewCellRadiusDamping, false);
		splitJoints[5].setTarget(unit / 2.0, 0.01);
		splitJoints[6].init(&splitPoints[1], &splitPoints[0], ws->c_NewCellRadiusStrength, ws->c_NewCellRadiusDamping, false);
		splitJoints[6].setTarget(unit / 2.0, 0.01);
		splitJoints[7].init(&splitPoints[1], &edgePoints[(5 + splitLocation) % AmountOfEdges], ws->c_NewCellRadiusStrength, ws->c_NewCellRadiusDamping, false);
		splitJoints[7].setTarget(unit / 2.0, 0.01);

		splitJoints[8].init(&splitPoints[2], &center, ws->c_NewCellRadiusStrength, ws->c_NewCellRadiusDamping, false);
		splitJoints[8].setTarget(unit / 2.0, 0.01);
		splitJoints[9].init(&splitPoints[2], &splitPoints[0], ws->c_NewCellRadiusStrength, ws->c_NewCellRadiusDamping, false);
		splitJoints[9].setTarget(unit / 2.0, 0.01);
		splitJoints[10].init(&splitPoints[2], &edgePoints[(2 + splitLocation) % AmountOfEdges], ws->c_NewCellRadiusStrength, ws->c_NewCellRadiusDamping, false);
		splitJoints[10].setTarget(unit / 2.0, 0.01);

		splitPointsLength = 3;
		splitJointsLength = 11;
		stage = 5;
		return false;
	}
	else if (stage == 5)
	{
		splitJoints[4].deconstruct();
		splitJoints[4].init(&splitPoints[1], &splitPoints[2], ws->c_NewCellRadiusStrength, ws->c_NewCellRadiusDamping, false);
		splitJoints[4].setTarget(unit / 2.0, 0.01);

		splitPointsLength = 3;
		splitJointsLength = 11;
		stage = 6;
		return true;
	}
	return false;
}

int CellFrame::getStage()
{
	return stage;
}

void CellFrame::splitFrame(CellFrame* cell1, CellFrame* cell2)
{
	if (stage == 6)
	{
		cell1->edgePoints[(0 + splitLocation) % AmountOfEdges].setPlace(splitPoints[1].getPlace());
		cell1->edgePoints[(1 + splitLocation) % AmountOfEdges].setPlace(splitPoints[2].getPlace());
		cell1->edgePoints[(2 + splitLocation) % AmountOfEdges].setPlace(edgePoints[(2 + splitLocation) % AmountOfEdges].getPlace());
		cell1->edgePoints[(3 + splitLocation) % AmountOfEdges].setPlace(edgePoints[(3 + splitLocation) % AmountOfEdges].getPlace());
		cell1->edgePoints[(4 + splitLocation) % AmountOfEdges].setPlace(edgePoints[(4 + splitLocation) % AmountOfEdges].getPlace());
		cell1->edgePoints[(5 + splitLocation) % AmountOfEdges].setPlace(edgePoints[(5 + splitLocation) % AmountOfEdges].getPlace());

		cell2->edgePoints[(0 + splitLocation) % AmountOfEdges].setPlace(edgePoints[(0 + splitLocation) % AmountOfEdges].getPlace());
		cell2->edgePoints[(1 + splitLocation) % AmountOfEdges].setPlace(edgePoints[(1 + splitLocation) % AmountOfEdges].getPlace());
		cell2->edgePoints[(2 + splitLocation) % AmountOfEdges].setPlace(edgePoints[(2 + splitLocation) % AmountOfEdges].getPlace());
		cell2->edgePoints[(3 + splitLocation) % AmountOfEdges].setPlace(splitPoints[2].getPlace());
		cell2->edgePoints[(4 + splitLocation) % AmountOfEdges].setPlace(splitPoints[1].getPlace());
		cell2->edgePoints[(5 + splitLocation) % AmountOfEdges].setPlace(edgePoints[(5 + splitLocation) % AmountOfEdges].getPlace());

		cell1->edgePoints[(0 + splitLocation) % AmountOfEdges].setVelocity(splitPoints[1].getVelocity());
		cell1->edgePoints[(1 + splitLocation) % AmountOfEdges].setVelocity(splitPoints[2].getVelocity());
		cell1->edgePoints[(2 + splitLocation) % AmountOfEdges].setVelocity(edgePoints[2].getVelocity());
		cell1->edgePoints[(3 + splitLocation) % AmountOfEdges].setVelocity(edgePoints[3].getVelocity());
		cell1->edgePoints[(4 + splitLocation) % AmountOfEdges].setVelocity(edgePoints[4].getVelocity());
		cell1->edgePoints[(5 + splitLocation) % AmountOfEdges].setVelocity(edgePoints[5].getVelocity());

		cell2->edgePoints[(0 + splitLocation) % AmountOfEdges].setVelocity(edgePoints[(0 + splitLocation) % AmountOfEdges].getVelocity());
		cell2->edgePoints[(1 + splitLocation) % AmountOfEdges].setVelocity(edgePoints[(1 + splitLocation) % AmountOfEdges].getVelocity());
		cell2->edgePoints[(2 + splitLocation) % AmountOfEdges].setVelocity(edgePoints[(2 + splitLocation) % AmountOfEdges].getVelocity());
		cell2->edgePoints[(3 + splitLocation) % AmountOfEdges].setVelocity(splitPoints[2].getVelocity());
		cell2->edgePoints[(4 + splitLocation) % AmountOfEdges].setVelocity(splitPoints[1].getVelocity());
		cell2->edgePoints[(5 + splitLocation) % AmountOfEdges].setVelocity(edgePoints[(5 + splitLocation) % AmountOfEdges].getVelocity());

		float volume1 = cell1->getVolume();
		float volume2 = cell2->getVolume();
		float volumeTotal = volume1 + volume2;

		for (int i = 0; i < WorldSettings::e_AmountOfParticles; i++)
		{
			cell1->particles[i] = particles[i] * volume1 / volumeTotal;
			cell2->particles[i] = particles[i] * volume2 / volumeTotal;
		}

		cell1->energy = energy * volume1 / volumeTotal;
		cell2->energy = energy * volume2 / volumeTotal;

		cell1->cacheParameters();
		cell2->cacheParameters();

		splitFrameHelper(cell2, splitLocation);
		splitFrameHelper(cell2, (splitLocation + 1) % AmountOfEdges);
		splitFrameHelper(cell2, (splitLocation + 5) % AmountOfEdges);

		splitFrameHelper(cell1, (splitLocation + 2) % AmountOfEdges);
		splitFrameHelper(cell1, (splitLocation + 3) % AmountOfEdges);
		splitFrameHelper(cell1, (splitLocation + 4) % AmountOfEdges);
	}
}

void CellFrame::splitFrameHelper(CellFrame * newCell, int own)
{
	if (connectedCells[own] != nullptr)
	{
		CellFrame* otherCell = connectedCells[own];
		for (int l = 0; l < AmountOfEdges; l++)
		{
			if (otherCell->connectedCells[l] != nullptr)
			{
				if (otherCell->connectedCells[l]->getId() == id)
				{
					int other = l;
					int ownNext = own + 1 == AmountOfEdges ? 0 : own + 1;
					int otherNext = other + 1 == AmountOfEdges ? 0 : other + 1;
					connectedCells[own] = nullptr;

					if (connectedCellsMaster[own])
					{
						connectedCellsJoints[own * 2].deconstruct();
						connectedCellsJoints[own * 2 + 1].deconstruct();
						connectedCellsMaster[own] = false;
					}
					else
					{
						otherCell->connectedCellsJoints[other * 2].deconstruct();
						otherCell->connectedCellsJoints[other * 2 + 1].deconstruct();
						otherCell->connectedCellsMaster[other] = false;
					}
					otherCell->connectedCells[other] = newCell;
					newCell->connectedCells[own] = otherCell;
					newCell->connectedCellsMaster[own] = true;
					newCell->connectedCellsJoints[own * 2].init(&newCell->edgePoints[own], &otherCell->edgePoints[otherNext], ws->c_NewCellRadiusStrength, ws->c_NewCellRadiusDamping, false);
					newCell->connectedCellsJoints[own * 2 + 1].init(&newCell->edgePoints[ownNext], &otherCell->edgePoints[other], ws->c_NewCellRadiusStrength, ws->c_NewCellRadiusDamping, false);
					newCell->connectedCellsJoints[own * 2].setTarget(0, 0.1);
					newCell->connectedCellsJoints[own * 2 + 1].setTarget(0, 0.1);
				}
			}
		}
	}
}

void CellFrame::applyPressure(float p)
{
	for (auto& joint : edgeJoints)
	{
		Vector jointLine = Vector(joint.getP1()->getPlace(), joint.getP2()->getPlace());
		float surface = jointLine.getLength();
		Vector normal = jointLine.getPerpendicularCounterClockwise().getUnit();
		normal.multiply(0.5*surface*p);
		joint.getP1()->addForce(normal);
		joint.getP2()->addForce(normal);
	}
}

double CellFrame::getSurface() const
{
	float total = 0.0;
	for (auto& joint : edgeJoints)
	{
		total+=Vector::getLength(joint.getP1()->getPlace(), joint.getP2()->getPlace()); 
	}
	return total;
}

float CellFrame::getVolume() const
{
	float total = 0;

	for (int i = 0; i < AmountOfEdges - 1; i++)
	{
		total += Shapes::surfaceTriangle(Vector::getLength(center.getPlace(), edgePoints[i].getPlace()),
			Vector::getLength(center.getPlace(), edgePoints[i + 1].getPlace()),
			Vector::getLength(edgePoints[i].getPlace(), edgePoints[i + 1].getPlace()));
	}
	total += Shapes::surfaceTriangle(Vector::getLength(center.getPlace(), edgePoints[0].getPlace()),
		Vector::getLength(center.getPlace(), edgePoints[AmountOfEdges - 1].getPlace()),
		Vector::getLength(edgePoints[0].getPlace(), edgePoints[AmountOfEdges - 1].getPlace()));
	return total;
}

bool CellFrame::isBroken()const {
	//distance check
	//for (int i = 0; i < AmountOfEdges; i++) {
	//	if (Vector(center.getPlace(), edgePoints[i]->getPlace()).getLength() > radius * world->ws.maxExpantion) {
	//		return true;
	//	}
	//}
	//Edge cross check
	for (int i = 0; i < AmountOfEdges; i++) {
		for (int j = 0; j < AmountOfEdges - 1; j++) {
			Vector intersection(0.0, 0.0);
			if (Shapes::lineSegementsIntersect(center.getPlace(), edgePoints[i].getPlace()
				, edgePoints[j].getPlace(), edgePoints[j + 1].getPlace()
				, intersection, 0.001)) {
				return true;
			}
		}
		Vector intersection(0.0, 0.0);
		if (Shapes::lineSegementsIntersect(center.getPlace(), edgePoints[i].getPlace()
			, edgePoints[0].getPlace(), edgePoints[AmountOfEdges - 1].getPlace()
			, intersection, 0.001)) {
			return true;
		}
	}
	return false;
}

const Vector & CellFrame::getEdgeJoint(const int i, const bool p1) const
{
	if (p1)
	{
		return edgeJoints[i].getP1()->getPlace();
	}
	else
	{
		return edgeJoints[i].getP2()->getPlace();
	}
}
const Vector & CellFrame::getRadiusJoint(const int i, const bool p1) const
{
	if (p1)
	{
		return radiusJoints[i].getP1()->getPlace();
	}
	else
	{
		return radiusJoints[i].getP2()->getPlace();
	}
}
const Vector & CellFrame::getSplitJoint(const int i, const bool p1) const
{
	if (p1)
	{
		return splitJoints[i].getP1()->getPlace();
	}
	else
	{
		return splitJoints[i].getP2()->getPlace();
	}
}
int CellFrame::getAmountOfEdgeJoints() const
{
	return AmountOfEdges;
}
int CellFrame::getAmountOfradiusJoints() const
{
	return AmountOfEdges;
}
int CellFrame::getAmountOfSplitJoints() const
{
	return splitJointsLength;
}

const Vector& CellFrame::getTailPoint(const int i, const int j) const
{
	if (j)
	{
		return tailJoints[i].getP1()->getPlace();
	}
	return tailJoints[i].getP2()->getPlace();
}
int CellFrame::getAmountOfTailJoints() const
{
	return tailLength*5;
}

const Joint& CellFrame::getEdgeEdge(int i) const
{
	return edgeJoints[i];
}

const Joint& CellFrame::getTailEdge(int i) const
{
	return tailJoints[i];
}

const Vector& CellFrame::getCenter()const
{
	return center.getPlace();
}

long CellFrame::getId() const
{
	return id;
}

/*

void CellFrame::lineCellCollision(Line* line) {
//check if its around the box of the line, otherwise dont check
if (center->getPlace()->getX() + (radius * 3) > line->getLeft() && center->getPlace()->getX() - (radius * 3) < line->getRight() &&
center->getPlace()->getY() + (radius * 3) > line->getTop() && center->getPlace()->getY() - (radius * 3) < line->getBottom())
{
//normal collision handeling
lineCellForce(*line->getV1(), *line->getV2());
}
}
void CellFrame::lineCellForce(Vector&perpendicular1, Vector&perpendicular2)
{
for (int k = 0; k < Cell::AmountOfEdges; k++)
{
//check if the radius of a cell goes through a line
Vector intersection(0.0, 0.0);
if (this->lineSegementsIntersect(perpendicular1, perpendicular2, *center->getPlace(), *edgePoints[k]->getPlace(), intersection))
{
Vector line(perpendicular1, perpendicular2); //vector of line
Vector perpendicular = line.getPerpendicularClockwise().makeUnit(); //perpendicular to line

//make a line through the place of the cell-corner and perpendicular to the line
Vector p1(*edgePoints[k]->getPlace() + (perpendicular * 1000));
Vector p2(*edgePoints[k]->getPlace() - (perpendicular * 1000));

//calculate the intersection and set it as the new place of the cell-corner
Vector correctedIntersection(0.0, 0.0);
lineSegementsIntersect(p1, p2, perpendicular1 - (line * 1000), perpendicular2 + (line * 1000), correctedIntersection);
float moveDistance = Vector::getLength(&correctedIntersection, edgePoints[k]->getPlace());
//c->edgePoints[k]->setPlace(correctedIntersection);

//calculate the radius so you can calculate the angle between the perpendicular and the radius. Required to determain on which side of the line the center is
Vector radius(center->getPlace(), edgePoints[k]->getPlace());
radius.makeUnit();
float angle = acos(radius.dot(perpendicular))*180.0 / 3.1415926535897;

//calculate the normal force and add it
if (abs(angle) < 90)
{
perpendicular.multiply(moveDistance * -world->c_LineCellCollisionForce * edgePoints[k]->getMass() * 4);
}
else
{
perpendicular.multiply(moveDistance * world->c_LineCellCollisionForce * edgePoints[k]->getMass() * 4);
}
edgePoints[k]->addForce(perpendicular);

// ### Friction with sides ###
Vector velocity(edgePoints[k]->getVelocity());
if (velocity.getLength() != 0) {
Vector velocityUnit = velocity.getUnit();
line.makeUnit();

//calculate the direction
angle = acos(velocityUnit.dot(line))*180.0 / 3.1415926535897;
if (abs(angle) > 90)
{
line.multiply(-1);
}

//calculate the component of the velocity with the line as baseline
Vector velocityComponent(line*velocity.dot(line));

//calculate friction based on the velocity and the moveDistance
velocityComponent.multiply(-world->c_LineCellFriction * moveDistance);
edgePoints[k]->addForce(velocityComponent);
}
}
}
}
*/