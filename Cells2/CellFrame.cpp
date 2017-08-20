#include "CellFrame.h"

long Cell::idCounter = 0;

CellFrame::CellFrame(World * _world, const Vector& tCenter, const float radius)
{
	world = _world;
	world->stats_CellsCreated++;
	id = Cell::idCounter;
	idCounter++;

	float pointMass = 1;//old

	float x = tCenter.getX();
	float y = tCenter.getY();

	center.init(tCenter.getX(), tCenter.getY(), pointMass);

	edgePoints[0].init(x, y - 20 * radius / 20, pointMass);
	edgePoints[1].init(x + 17.32 * radius / 20, y - 10 * radius / 20, pointMass);
	edgePoints[2].init(x + 17.32 * radius / 20, y + 10 * radius / 20, pointMass);
	edgePoints[3].init(x, y + 20 * radius / 20, pointMass);
	edgePoints[4].init(x - 17.32 * radius / 20, y + 10 * radius / 20, pointMass);
	edgePoints[5].init(x - 17.32 * radius / 20, y - 10 * radius / 20, pointMass);

	radiusJoints[0].init(&center, &edgePoints[0], world->c_NewCellRadiusStrength, world->c_NewCellRadiusDamping, false);
	radiusJoints[1].init(&center, &edgePoints[1], world->c_NewCellRadiusStrength, world->c_NewCellRadiusDamping, false);
	radiusJoints[2].init(&center, &edgePoints[2], world->c_NewCellRadiusStrength, world->c_NewCellRadiusDamping, false);
	radiusJoints[3].init(&center, &edgePoints[3], world->c_NewCellRadiusStrength, world->c_NewCellRadiusDamping, false);
	radiusJoints[4].init(&center, &edgePoints[4], world->c_NewCellRadiusStrength, world->c_NewCellRadiusDamping, false);
	radiusJoints[5].init(&center, &edgePoints[5], world->c_NewCellRadiusStrength, world->c_NewCellRadiusDamping, false);

	edgeJoints[0].init(&edgePoints[0], &edgePoints[1], world->c_NewCellSideStrength, world->c_NewCellSideDamping, true);
	edgeJoints[1].init(&edgePoints[1], &edgePoints[2], world->c_NewCellSideStrength, world->c_NewCellSideDamping, true);
	edgeJoints[2].init(&edgePoints[2], &edgePoints[3], world->c_NewCellSideStrength, world->c_NewCellSideDamping, true);
	edgeJoints[3].init(&edgePoints[3], &edgePoints[4], world->c_NewCellSideStrength, world->c_NewCellSideDamping, true);
	edgeJoints[4].init(&edgePoints[4], &edgePoints[5], world->c_NewCellSideStrength, world->c_NewCellSideDamping, false);
	edgeJoints[5].init(&edgePoints[5], &edgePoints[0], world->c_NewCellSideStrength, world->c_NewCellSideDamping, true);

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

Cell::~Cell() {
	int startDisconnect = -1;
	for (int i = 0; i<AmountOfEdges; i++)
	{
		disconnectCells(i);
	}
}

Vector CellFrame::calcJointForces(const Vector& flow)
{
	center.calcForcesJoints();
	for (auto& point : edgePoints)
	{
		point.calcForcesJoints();
	}
	for (auto& point : tailPoints)
	{
		point.calcForcesJoints();
	}

	Vector frictionTotal(0.0, 0.0);
	for (auto& joint : edgeJoints)
	{
		frictionTotal.add(joint.calcFriction(flow));
	}
	for (auto& joint : tailJoints)
	{
		frictionTotal.add(joint.calcFriction(flow));
	}
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
	float massPoint = getMass() / (1 + AmountOfEdges + tail.size());
	center->applyForces(precision, backgroundFriction);
	for (auto point : edgePoints)
	{
		if (id == point->getBelongsTo()) {
			point->applyForces(precision, backgroundFriction);
		}
	}
	for (auto point : tail)
	{
		if (id == point->getBelongsTo()) {
			point->applyForces(precision, backgroundFriction);
		}
	}
}

void CellFrame::cellCellCollision(Cell* other)
{
	//calculate distance between 2 centers, if its shorter than the combined size, start detailed collision
	Vector connectedLine(center->getPlace(), other->center->getPlace());
	float distance = connectedLine.getLength();
	float combinedSize = radius + other->radius;
	if ((combinedSize * 2) > distance)
	{
		this->cellCellForce(other);
		other->cellCellForce(this);
	}
}

void CellFrame::cellCellForce(Cell* other)
{
	for (int i = 0; i < Cell::AmountOfEdges; i++)
	{
		if (connectedCells[i] != other)
		{
			if (Vector::getLength(other->center->getPlace(), edgePoints[i]->getPlace()) < other->radius * 2)
			{
				for (int j = 0; j < Cell::AmountOfEdges; j++)
				{
					Vector intersection(0.0, 0.0);
					if (this->lineSegementsIntersect(*center->getPlace(), *edgePoints[i]->getPlace(),
						*other->edgeJoints[j]->p1->getPlace(), *other->edgeJoints[j]->p2->getPlace(), intersection))
					{
						Vector overlap(intersection, edgePoints[i]->getPlace());
						Vector force(other->center->getPlace(), edgePoints[i]->getPlace());
						force.makeUnit();
						force.multiply(overlap.getLength() * world->c_CellCellCollisionForce * edgePoints[i]->getMass());
						edgePoints[i]->addForce(force);

						float l1 = Vector::getLength(intersection, other->edgeJoints[j]->p1->getPlace());
						float l2 = Vector::getLength(intersection, other->edgeJoints[j]->p2->getPlace());

						other->edgeJoints[j]->p1->addForce(force*((-l2) / (l1 + l2)));
						other->edgeJoints[j]->p2->addForce(force*((-l1) / (l1 + l2)));

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
				}
			}
		}
	}
}

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

bool CellFrame::isBroken()const {
	//distance check
	for (int i = 0; i < AmountOfEdges; i++) {
		if (Vector(center->getPlace(), edgePoints[i]->getPlace()).getLength() > radius * world->ws.maxExpantion) {
			return true;
		}
	}
	//Edge cross check
	for (int i = 0; i < AmountOfEdges; i++) {
		for (int j = 0; j < AmountOfEdges - 1; j++) {
			Vector intersection(0.0, 0.0);
			if (lineSegementsIntersect(*center->getPlace(), *edgePoints[i]->getPlace()
				, *edgePoints[j]->getPlace(), *edgePoints[j + 1]->getPlace()
				, intersection, 0.001)) {
				return true;
			}
		}
		Vector intersection(0.0, 0.0);
		if (lineSegementsIntersect(*center->getPlace(), *edgePoints[i]->getPlace()
			, *edgePoints[0]->getPlace(), *edgePoints[AmountOfEdges - 1]->getPlace()
			, intersection, 0.001)) {
			return true;
		}
	}
	return false;
}



Vector* CellFrame::getEdgePoint(const int i)const
{
	return edgePoints[i]->getPlace();
}

Vector* CellFrame::getTailPoint(const int i, const int j) const
{
	if (j)
	{
		return tailJoints[i]->getP1()->getPlace();
	}
	return tailJoints[i]->getP2()->getPlace();
}

int CellFrame::getEdgePointId(const int i) const
{
	return edgePoints[i]->getID();
}

shared_ptr<Point> CellFrame::getEdgePointPtr(const int i) const
{
	return edgePoints[i];
}

int CellFrame::getAmountOfEdgeEdges() const
{
	return AmountOfEdges;
}

int CellFrame::getAmountOfTailEdges() const
{
	return tailJoints.size();
}

Joint* CellFrame::getEdgeEdge(int i) const
{
	return edgeJoints[i];
}

Joint* CellFrame::getTailEdge(int i) const
{
	return tailJoints[i];
}

Vector* CellFrame::getCenter()const
{
	return center->getPlace();
}

int CellFrame::getCenterId() const
{
	return center->getID();
}

shared_ptr<Point> CellFrame::getCenterPtr() const
{
	return center;
}

long CellFrame::getId() const
{
	return id;
}