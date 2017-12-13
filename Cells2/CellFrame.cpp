#include "CellFrame.h"
#include "Block.h"

long CellFrame::idCounter = 0;

const float PI = 3.14159265358979f;

CellFrame::CellFrame(WorldSettings * _ws, const Vector& tCenter)
{
	ws = _ws;
	ws->stats_CellsCreated++;
	id = CellFrame::idCounter++;

	tailLength = 0;
	tailLocation = 4;
	hasTailEnd = false;
	stage = 0;
	splitLocation = 0;
	unit = ws->defaultRadius;
	splitPointsLength = 0;
	splitJointsLength = 0;

	registerPoint(&center, new Point(ws, tCenter.getX(), tCenter.getY(), ws->defaultMass, true));
	for (int i = 0; i < AmountOfEdges; i++)
	{
		registerPoint(&edgePoints[i], new Point(ws, tCenter.getX() + (cos(i * PI / 3.0)*unit), 
			tCenter.getY() + (sin(i * PI / 3.0)*unit), ws->defaultMass, true));
	}

	for (int i = 0; i < AmountOfEdges; i++)
	{
		radiusJoints[i].init(ws, center, edgePoints[i]);
		edgeJoints[i].init(ws, edgePoints[i], edgePoints[loc(i+1)]);

		//connectedCellsMaster[i] = false;
		//connectedCells[i] = nullptr;
	}

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
CellFrame::CellFrame(CellFrame* p)
{
	if (p->stage == 6)
	{
		ws = p->ws;
		ws->stats_CellsCreated++;
		id = CellFrame::idCounter++;

		tailLength = 0;
		tailLocation = 4;
		hasTailEnd = false;
		stage = 0;
		splitLocation = p->splitLocation;
		unit = p->unit;
		splitPointsLength = 0;
		splitJointsLength = 0;

		center = p->splitPoints[0];

		edgePoints[p->loc(0)] = p->edgePoints[p->loc(0)];
		edgePoints[p->loc(1)] = p->edgePoints[p->loc(1)];

		edgePoints[p->loc(4)] = p->splitPoints[1];
		edgePoints[p->loc(3)] = p->splitPoints[2];

		p->registerPoint(&p->edgePoints[p->loc(0)], new Point(edgePoints[p->loc(4)], 0.5, true));
		p->registerPoint(&p->edgePoints[p->loc(1)], new Point(edgePoints[p->loc(3)], 0.5, true));

		registerPoint(&edgePoints[p->loc(5)], new Point(p->edgePoints[p->loc(5)], 0.5, true));
		registerPoint(&edgePoints[p->loc(2)], new Point(p->edgePoints[p->loc(2)], 0.5, true));

		for (int i = 0; i < 6; i++)
		{
			p->radiusJoints[p->loc(i)].init(ws, p->center, p->edgePoints[p->loc(i)], unit, 0.01);
			p->edgeJoints[p->loc(i)].init(ws, p->edgePoints[p->loc(i)], p->edgePoints[p->loc(i+1)], unit, 0.01);

			radiusJoints[loc(i)].init(ws, center, edgePoints[loc(i)], unit, 0.01);
			edgeJoints[loc(i)].init(ws, edgePoints[loc(i)], edgePoints[loc(i + 1)], unit, 0.01);

			//connectedCellsMaster[i] = false;
			//connectedCells[i] = nullptr;
		}

		for (int i = 0; i < p->splitJointsLength; i++)
		{
			p->splitJoints[i].deconstruct();
		}

		p->splitPointsLength = 0;
		p->splitJointsLength = 0;
		p->stage = 0;
	}
	else
	{
		throw "Parent is not ready";
	}
}
CellFrame::~CellFrame() {
	for (int i = 0; i < AmountOfEdges; i++)
	{
		//disconnectCells(i);
		if (armJoints[i].isActive())
		{
			armJoints[i].deconstruct();
		}
	}
	center->setOwned(false);
	for (int i = 0; i < 6; i++)
	{
		edgePoints[i]->setOwned(false);
	}
	for (int i = 0; i < splitPointsLength; i++)
	{
		splitPoints[i]->setOwned(false);
	}
	if(pointsToRegister.size()>0)
	{
		throw "Not all points saved";
	}
}



void CellFrame::jointLogic()
{
	for (auto& joint : radiusJoints)
	{
		joint.logic(ws->precision);
	}
	for (auto& joint : edgeJoints)
	{
		joint.logic(ws->precision);
	}
	for (int i = 0; i < tailLength*5; i++) 
	{
		tailJoints[i].logic(ws->precision);
	}
	if (hasTailEnd) 
	{
		tailEndJoints[0].logic(ws->precision);
		tailEndJoints[1].logic(ws->precision);
		tailEndJoints[2].logic(ws->precision);
		tailEndJoints[3].logic(ws->precision);
	}
	for (int i = 0; i < AmountOfEdges; i++) 
	{
		if (armJoints[i].isActive())
		{
			armJoints[i].logic(ws->precision);
		}
		/*if (connectedCellsMaster[i])
		{
			connectedCellsJoints[i * 2 + 0].logic(ws->precision);
			connectedCellsJoints[i * 2 + 1].logic(ws->precision);
		}*/
	}
	for (int i = 0; i < splitJointsLength; i++) {
		splitJoints[i].logic(ws->precision);
	}
}
void CellFrame::calcJointForces()
{
	center->calcForceJoints();
	for (auto& point : edgePoints)
	{
		point->calcForceJoints();
	}
	for (int i = 0; i < splitPointsLength; i++)
	{
		splitPoints[i]->calcForceJoints();
	}
	for (int i = 0; i < tailLength * 2; i++)
	{
		tailPoints[i]->calcForceJoints();
	}
	if (hasTailEnd)
	{
		tailEndPoint->calcForceJoints();
	}
}

/*
bool CellFrame::connectCells(CellFrame * otherCell)
{
	int own = 0;
	int other = 0;
	float distance = 10000;
	for (int i = 0; i<AmountOfEdges; i++)
	{
		for (int j = 0; j<AmountOfEdges; j++)
		{
			float d = Vector::getLength(edgePoints[i]->getPlace(), otherCell->edgePoints[(j+1)%AmountOfEdges]->getPlace()) 
				* Vector::getLength(edgePoints[(i+1)%AmountOfEdges]->getPlace(), otherCell->edgePoints[j]->getPlace());
			if (d<distance && connectedCells[i] == nullptr &&  otherCell->connectedCells[j] == nullptr)
			{
				if (!connectedToPoint[i * 2] && !connectedToPoint[i * 2 + 1])
				{
					if (i != tailLocation || (!hasTailEnd && tailLength == 0))
					{
						if (j != otherCell->tailLocation || (!otherCell->hasTailEnd && otherCell->tailLength == 0))
						{
							own = i;
							other = j;
							distance = d;
						}
					}
				}
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
		
		connectedCellsJoints[own * 2].init(ws, edgePoints[own], otherCell->edgePoints[otherNext], 0, 0.1);
		connectedCellsJoints[own * 2 + 1].init(ws, edgePoints[ownNext], otherCell->edgePoints[other], 0, 0.1);

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
*/

void CellFrame::startSplit(const int location)
{
	if (stage == 0) 
	{
		splitLocation = location;
		stage = 1;
	}
}
void CellFrame::nextStage()
{
	if (stage == 1) 
	{
		splitJoints[0].init(ws, edgePoints[loc(4)], edgePoints[loc(0)], 2.5 * unit, 0.01);
		splitJoints[1].init(ws, edgePoints[loc(3)], edgePoints[loc(1)], 2.5 * unit, 0.01);
		splitJointsLength = 2;
		stage++;
	}
	else if (stage == 2)
	{
		registerPoint(&splitPoints[0], new Point(center, 0.5, true));

		splitJoints[2].init(ws, edgePoints[loc(5)], splitPoints[0], unit, 0.01);
		splitJoints[3].init(ws, edgePoints[loc(2)], splitPoints[0], unit, 0.01);

		radiusJoints[loc(0)].changeFromTo(center, splitPoints[0]);
		radiusJoints[loc(1)].changeFromTo(center, splitPoints[0]);

		splitPointsLength = 1;
		splitJointsLength = 4;
		stage++;
	}
	else if (stage == 3)
	{
		splitJoints[4].init(ws, center, splitPoints[0], unit, 0.01);

		splitPointsLength = 1;
		splitJointsLength = 5;
		stage = 4;
	}
	else if (stage == 4)
	{
		registerPoint(&splitPoints[1], new Point(edgePoints[loc(5)], 0.5, true));
		registerPoint(&splitPoints[2], new Point(edgePoints[loc(2)], 0.5, true));

		splitJoints[5].init(ws, splitPoints[1], center, unit / 2.0, 1);
		splitJoints[6].init(ws, splitPoints[1], splitPoints[0], unit / 2.0, 1);
		splitJoints[7].init(ws, splitPoints[1], edgePoints[loc(5)], unit / 2.0, 1);

		splitJoints[8].init(ws, splitPoints[2], center, unit / 2.0, 1);
		splitJoints[9].init(ws, splitPoints[2], splitPoints[0], unit / 2.0, 1);
		splitJoints[10].init(ws, splitPoints[2], edgePoints[loc(2)], unit / 2.0, 1);

		splitPointsLength = 3;
		splitJointsLength = 11;
		stage = 5;
	}
	else if (stage == 5)
	{
		splitJoints[4].init(ws, splitPoints[1], splitPoints[2], unit / 2.0, 0.01);
		stage = 6;
	}
}
int CellFrame::getStage()
{
	return stage;
}
/*void CellFrame::growTail()
{
	if (connectedCells[tailLocation] == nullptr)
	{
		if (hasTailEnd)
		{
			if (tailLength < MaxTailLength)
			{
				hasTailEnd = false;
				tailLength++;
				tailEndJoints[0].deconstruct();
				tailEndJoints[1].deconstruct();
				tailEndJoints[2].deconstruct();
				tailEndJoints[3].deconstruct();
				tailEndPoint.deconstruct();
				tailPoints[(tailLength - 1) * 2].init(ws, tailEndPoint.getPlace().getX(), tailEndPoint.getPlace().getY(), 2.0, true);
				tailPoints[(tailLength - 1) * 2 + 1].init(ws, tailEndPoint.getPlace().getX(), tailEndPoint.getPlace().getY(), 2.0, true);
				if (tailLength == 1)
				{
					tailJoints[0].init(&tailPoints[0], &edgePoints[(tailLocation + 1) % AmountOfEdges], ws->c_NewCellRadiusStrength, ws->c_NewCellRadiusDamping, true);
					tailJoints[1].init(&edgePoints[tailLocation], &tailPoints[1], ws->c_NewCellRadiusStrength, ws->c_NewCellRadiusDamping, true);
					tailJoints[2].init(&edgePoints[tailLocation], &tailPoints[0], ws->c_NewCellRadiusStrength, ws->c_NewCellRadiusDamping, false);
					tailJoints[3].init(&edgePoints[(tailLocation + 1) % AmountOfEdges], &tailPoints[1], ws->c_NewCellRadiusStrength, ws->c_NewCellRadiusDamping, false);
					tailJoints[4].init(&tailPoints[1], &tailPoints[0], ws->c_NewCellRadiusStrength, ws->c_NewCellRadiusDamping, true);
				}
				else
				{
					tailJoints[(tailLength - 1) * 5 + 0].init(&tailPoints[(tailLength - 1) * 2 + 0], &tailPoints[(tailLength - 2) * 2 + 0], ws->c_NewCellRadiusStrength, ws->c_NewCellRadiusDamping, true);
					tailJoints[(tailLength - 1) * 5 + 1].init(&tailPoints[(tailLength - 2) * 2 + 1], &tailPoints[(tailLength - 1) * 2 + 1], ws->c_NewCellRadiusStrength, ws->c_NewCellRadiusDamping, true);
					tailJoints[(tailLength - 1) * 5 + 2].init(&tailPoints[(tailLength - 1) * 2 + 0], &tailPoints[(tailLength - 2) * 2 + 1], ws->c_NewCellRadiusStrength, ws->c_NewCellRadiusDamping, false);
					tailJoints[(tailLength - 1) * 5 + 3].init(&tailPoints[(tailLength - 1) * 2 + 1], &tailPoints[(tailLength - 2) * 2 + 0], ws->c_NewCellRadiusStrength, ws->c_NewCellRadiusDamping, false);
					tailJoints[(tailLength - 1) * 5 + 4].init(&tailPoints[(tailLength - 1) * 2 + 1], &tailPoints[(tailLength - 1) * 2 + 0], ws->c_NewCellRadiusStrength, ws->c_NewCellRadiusDamping, true);
				}
				tailJoints[(tailLength - 1) * 5 + 0].setTarget(1 * unit, 0.01);
				tailJoints[(tailLength - 1) * 5 + 1].setTarget(1 * unit, 0.01);
				tailJoints[(tailLength - 1) * 5 + 2].setTarget(1.5 * unit, 0.05);
				tailJoints[(tailLength - 1) * 5 + 3].setTarget(1.5 * unit, 0.05);
				tailJoints[(tailLength - 1) * 5 + 4].setTarget(0.5 * unit, 0.01);
			}
		}
		else
		{
			hasTailEnd = true;
			if (tailLength == 0)
			{
				Vector place = Vector::getAverage(edgePoints[tailLocation].getPlace(), edgePoints[(tailLocation + 1) % AmountOfEdges].getPlace());
				tailEndPoint.init(ws, place.getX(), place.getY(), 2.0, true);
				tailEndJoints[0].init(&tailEndPoint, &edgePoints[tailLocation], ws->c_NewCellRadiusStrength, ws->c_NewCellRadiusDamping, true);
				tailEndJoints[1].init(&edgePoints[(tailLocation + 1) % AmountOfEdges], &tailEndPoint, ws->c_NewCellRadiusStrength, ws->c_NewCellRadiusDamping, true);
				if (splitLocation == tailLocation || (splitLocation + 1) % AmountOfEdges == tailLocation || (splitLocation + 5) % AmountOfEdges == tailLocation)
				{
					if (stage > 1)
					{
						tailEndJoints[2].init(&splitPoints[0], &tailEndPoint, ws->c_NewCellRadiusStrength, ws->c_NewCellRadiusDamping, false);
						tailEndJoints[3].init(&splitPoints[0], &tailEndPoint, ws->c_NewCellRadiusStrength, ws->c_NewCellRadiusDamping, false);
					}
					else
					{
						tailEndJoints[2].init(&center, &tailEndPoint, ws->c_NewCellRadiusStrength, ws->c_NewCellRadiusDamping, false);
						tailEndJoints[3].init(&center, &tailEndPoint, ws->c_NewCellRadiusStrength, ws->c_NewCellRadiusDamping, false);
					}
				}
				else
				{
					tailEndJoints[2].init(&center, &tailEndPoint, ws->c_NewCellRadiusStrength, ws->c_NewCellRadiusDamping, false);
					tailEndJoints[3].init(&center, &tailEndPoint, ws->c_NewCellRadiusStrength, ws->c_NewCellRadiusDamping, false);
				}
			}
			else if (tailLength == 1)
			{
				Vector place = Vector::getAverage(tailPoints[(tailLength - 1) * 2].getPlace(), tailPoints[(tailLength - 1) * 2 + 1].getPlace());
				tailEndPoint.init(ws, place.getX(), place.getY(), 2.0, true);
				tailEndJoints[0].init(&tailEndPoint, &tailPoints[(tailLength - 1) * 2], ws->c_NewCellRadiusStrength, ws->c_NewCellRadiusDamping, true);
				tailEndJoints[1].init(&tailPoints[(tailLength - 1) * 2 + 1], &tailEndPoint, ws->c_NewCellRadiusStrength, ws->c_NewCellRadiusDamping, true);
				tailEndJoints[2].init(&edgePoints[tailLocation], &tailEndPoint, ws->c_NewCellRadiusStrength, ws->c_NewCellRadiusDamping, false);
				tailEndJoints[3].init(&edgePoints[(tailLocation + 1) % AmountOfEdges], &tailEndPoint, ws->c_NewCellRadiusStrength, ws->c_NewCellRadiusDamping, false);
				//tailJoints[4].setFriction(false);
			}
			else
			{
				Vector place = Vector::getAverage(tailPoints[(tailLength - 1) * 2].getPlace(), tailPoints[(tailLength - 1) * 2 + 1].getPlace());
				tailEndPoint.init(ws, place.getX(), place.getY(), 2.0, true);
				tailEndJoints[0].init(&tailEndPoint, &tailPoints[(tailLength - 1) * 2], ws->c_NewCellRadiusStrength, ws->c_NewCellRadiusDamping, true);
				tailEndJoints[1].init(&tailPoints[(tailLength - 1) * 2 + 1], &tailEndPoint, ws->c_NewCellRadiusStrength, ws->c_NewCellRadiusDamping, true);
				tailEndJoints[2].init(&tailPoints[(tailLength - 2) * 2], &tailEndPoint, ws->c_NewCellRadiusStrength, ws->c_NewCellRadiusDamping, false);
				tailEndJoints[3].init(&tailPoints[(tailLength - 2) * 2 + 1], &tailEndPoint, ws->c_NewCellRadiusStrength, ws->c_NewCellRadiusDamping, false);
				//tailJoints[(tailLength - 1) * 5 + 4].setFriction(false);
			}
			tailEndJoints[0].setTarget(1.5 * unit, 0.01);
			tailEndJoints[1].setTarget(1.5 * unit, 0.01);
			tailEndJoints[2].setTarget(2.5 * unit, 0.01);
			tailEndJoints[3].setTarget(2.5 * unit, 0.01);
		}
	}
}*/

void CellFrame::setTailFibers(int i, float left, float right, float cross)
{
	tailJoints[i * 5 + 0].setTarget((1+left) * unit, 0.01);
	tailJoints[i * 5 + 1].setTarget((1+right) * unit, 0.01);
	tailJoints[i * 5 + 2].setTarget((1.5+left) * unit, 0.05);
	tailJoints[i * 5 + 3].setTarget((1.5+right) * unit, 0.05);
	tailJoints[i * 5 + 4].setTarget((0.5+cross) * unit, 0.01);
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
			if (Shapes::lineSegementsIntersect(center->getPlace(), edgePoints[i]->getPlace()
				, edgePoints[j]->getPlace(), edgePoints[j + 1]->getPlace()
				, intersection, 0.001)) {
				return true;
			}
		}
		Vector intersection(0.0, 0.0);
		if (Shapes::lineSegementsIntersect(center->getPlace(), edgePoints[i]->getPlace()
			, edgePoints[0]->getPlace(), edgePoints[AmountOfEdges - 1]->getPlace()
			, intersection, 0.001)) {
			return true;
		}
	}
	return false;
}

Point * CellFrame::getNextNewPoint()
{
	if (pointsToRegister.size() > 0)
	{
		Point* p = pointsToRegister[0];
		pointsToRegister.erase(pointsToRegister.begin());
		return p;
	}
	else
	{
		return nullptr;
	}
}

Point* CellFrame::getPoint(int i)
{
	if (i == 0)
	{
		return center;
	}
	i--;
	if (i < AmountOfEdges)
	{
		return edgePoints[i];
	}
	i -= AmountOfEdges;
	if (i < (tailLength * 2))
	{
		return tailPoints[i];
	}
	i -= tailLength * 2;
	if (i == 0 && hasTailEnd)
	{
		return tailEndPoint;
	}
	i -= hasTailEnd ? 1 : 0;
	return splitPoints[i];
}
const Vector & CellFrame::getPointPlace(int i)
{
	return getPoint(i)->getPlace();
}
const float CellFrame::getRadius(int i)
{
	return getPoint(i)->getRadius();
}
const int CellFrame::getAmountOfPoints() const
{
	return 1 + AmountOfEdges + (tailLength * 2) + (hasTailEnd ? 1 : 0) + splitPointsLength;
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
const Vector & CellFrame::getTailJoint(const int i, const bool p1) const
{
	if (i < tailLength * 5)
	{
		if (p1)
		{
			return tailJoints[i].getP1()->getPlace();
			//return tailJoints[(i / 2) * 5 + (i % 2)].getP1()->getPlace();
		}
		else
		{
			return tailJoints[i].getP2()->getPlace();
			//return tailJoints[(i / 2) * 5 + (i % 2)].getP2()->getPlace();
		}
	}
	else
	{
		if (p1)
		{
			return tailEndJoints[i - tailLength * 5].getP1()->getPlace();
		}
		else
		{
			return tailEndJoints[i - tailLength * 5].getP2()->getPlace();
		}
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
int CellFrame::getAmountOfTailJoints() const
{
	//return tailLength * 2 + (hasTailEnd ? 4 : 0);
	return tailLength * 5 + (hasTailEnd ? 4 : 0);
}

const Joint& CellFrame::getEdgeEdge(int i) const
{
	return edgeJoints[i];
}
const Joint& CellFrame::getTailEdge(int i) const
{
	if (i < tailLength * 5)
	{
		return tailJoints[i];
	}
	else
	{
		return tailEndJoints[i - 5 * tailLength];
	}
}
const Vector& CellFrame::getCenter()const
{
	return center->getPlace();
}

long CellFrame::getId() const
{
	return id;
}

int CellFrame::loc(int i)
{
	return (i + splitLocation) % AmountOfEdges;
}

void CellFrame::registerPoint(Point ** place, Point * point)
{
	pointsToRegister.push_back(point);
	*place = point;
}
