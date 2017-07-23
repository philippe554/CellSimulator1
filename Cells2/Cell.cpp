#include "Cell.h"

int Cell::idCounter = 0;

Cell::Cell(shared_ptr<DNA> tDna, World*tWorld, Vector tCenter, double tRadius)
: Reactor(&tWorld->ws, tRadius*tRadius*3.1415926535897)
{
	dna = tDna;
	world = tWorld;
	world->stats_CellsCreated++;
	id = Cell::idCounter;
	idCounter++;

	radius = tRadius;
	double pointMass = getVolume() / (amountEdges + 1.0);

	Chunk* chunk = world->findChunk_C(world->calcChunk(tCenter.getX()), world->calcChunk(tCenter.getY()));

	center = Point::MakePoint(chunk, tCenter.getX(), tCenter.getY(), pointMass, id);

	double x = tCenter.getX();
	double y = tCenter.getY();

	edgePoints[0] = Point::MakePoint(chunk, x, y - 20 * radius / 20, pointMass, id);
	edgePoints[1] = Point::MakePoint(chunk, x + 17.32 * radius / 20, y - 10 * radius / 20, pointMass, id);
	edgePoints[2] = Point::MakePoint(chunk, x + 17.32 * radius / 20, y + 10 * radius / 20, pointMass, id);
	edgePoints[3] = Point::MakePoint(chunk, x, y + 20 * radius / 20, pointMass, id);
	edgePoints[4] = Point::MakePoint(chunk, x - 17.32 * radius / 20, y + 10 * radius / 20, pointMass, id);
	edgePoints[5] = Point::MakePoint(chunk, x - 17.32 * radius / 20, y - 10 * radius / 20, pointMass, id);

	radiusJoints[0] = new Joint(center, edgePoints[0], world->c_NewCellRadiusStrength, world->c_NewCellRadiusDamping, false, 0, id);
	radiusJoints[1] = new Joint(center, edgePoints[1], world->c_NewCellRadiusStrength, world->c_NewCellRadiusDamping, false, 0, id);
	radiusJoints[2] = new Joint(center, edgePoints[2], world->c_NewCellRadiusStrength, world->c_NewCellRadiusDamping, false, 0, id);
	radiusJoints[3] = new Joint(center, edgePoints[3], world->c_NewCellRadiusStrength, world->c_NewCellRadiusDamping, false, 0, id);
	radiusJoints[4] = new Joint(center, edgePoints[4], world->c_NewCellRadiusStrength, world->c_NewCellRadiusDamping, false, 0, id);
	radiusJoints[5] = new Joint(center, edgePoints[5], world->c_NewCellRadiusStrength, world->c_NewCellRadiusDamping, false, 0, id);

	edgeJoints[0] = new Joint(edgePoints[0], edgePoints[1], world->c_NewCellSideStrength, world->c_NewCellSideDamping, true, 0, id);
	edgeJoints[1] = new Joint(edgePoints[1], edgePoints[2], world->c_NewCellSideStrength, world->c_NewCellSideDamping, true, 0, id);
	edgeJoints[2] = new Joint(edgePoints[2], edgePoints[3], world->c_NewCellSideStrength, world->c_NewCellSideDamping, true, 0, id);
	edgeJoints[3] = new Joint(edgePoints[3], edgePoints[4], world->c_NewCellSideStrength, world->c_NewCellSideDamping, true, 0, id);
	edgeJoints[4] = new Joint(edgePoints[4], edgePoints[5], world->c_NewCellSideStrength, world->c_NewCellSideDamping, false, 0, id);
	edgeJoints[5] = new Joint(edgePoints[5], edgePoints[0], world->c_NewCellSideStrength, world->c_NewCellSideDamping, true, 0, id);

	for (int i = 0; i < dna->tail.getAmountOfRows() - 1; i++)
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

	tailCounter = 0;

	outerMembrane = new Membrane(dna->membrane);
	temperature = ws->defaultTemperature; //TODO: get this temp from somewhere

	for (int i = 0; i < amountEdges; i++)
	{
		connectedCells[i] = nullptr;
	}

	particles[WorldSettings::p_hydrogen] = 500;
	particles[WorldSettings::p_carbon] = 40;
	particles[WorldSettings::p_oxygen] = 10;
	particles[WorldSettings::p_nitrogen] = 5;
}

Cell::~Cell(){
	int startDisconnect = -1;
	for(int i=0;i<amountEdges;i++)
	{
		disconnectCells(i);
	}
	for (auto joint : radiusJoints) 
	{
		if (joint != nullptr) 
		{
			delete joint;
			joint = nullptr;
		}
	}
	for (auto joint : edgeJoints) 
	{
		if (joint != nullptr) 
		{
			delete joint;
			joint = nullptr;
		}
	}
	for (auto joint : tailJoints)
	{
		if (joint != nullptr)
		{
			delete joint;
			joint = nullptr;
		}
	}
	delete outerMembrane;
}

void Cell::calcJointForces(const Vector& flow)
{
	center->calcForcesJoints();
	for (auto point : edgePoints)
	{
		if (id == point->getBelongsTo()) 
		{
			point->calcForcesJoints();
		}
	}
	for (auto point : tail)
	{
		if (id == point->getBelongsTo())
		{
			point->calcForcesJoints();
		}
	}
	for (auto joint : edgeJoints)
	{
		joint->calcFriction(flow);
	}
	for (auto joint : tailJoints)
	{
		joint->calcFriction(flow);
	}
	tailCounter++;
	for (int i = 0; i < dna->tail.getAmountOfRows() - 1; i++)
	{
		for (int j = 0; j < 3; j++) {
			int index = i * 3 + j;
			float c1 = dna->tail.getElementScaled(i, 4, -1, 1);
			float c2 = dna->tail.getElementScaled(i, 5, -1, 1);
			float c3 = dna->tail.getElementScaled(i, 6, 20, 200);
			float c4 = dna->tail.getElementScaled(i, 7, -1, 1);

			tailJoints[i * 15 + j * 5 + 0]->setLength( c1*sin(tailCounter / c3 + c4) + tailJoints[i * 15 + j * 5 + 0]->originalLength);
			tailJoints[i * 15 + j * 5 + 1]->setLength( c1*sin(tailCounter / c3 + c4) + tailJoints[i * 15 + j * 5 + 1]->originalLength);
			tailJoints[i * 15 + j * 5 + 2]->setLength(-c1*sin(tailCounter / c3 + c4) + tailJoints[i * 15 + j * 5 + 2]->originalLength);
			tailJoints[i * 15 + j * 5 + 3]->setLength(-c1*sin(tailCounter / c3 + c4) + tailJoints[i * 15 + j * 5 + 3]->originalLength);
			tailJoints[i * 15 + j * 5 + 4]->setLength( c2*sin(tailCounter / c3 + c4) + tailJoints[i * 15 + j * 5 + 4]->originalLength);
		}
	}
	float c1 = dna->tail.getElementScaled(-1, 4, -1, 1);
	float c3 = dna->tail.getElementScaled(-1, 6, 30, 200);
	float c4 = dna->tail.getElementScaled(-1, 7, -1, 1);
	tailJoints[(dna->tail.getAmountOfRows() - 1) * 15]->setLength( c1*sin(tailCounter / c3 + c4) + tailJoints[(dna->tail.getAmountOfRows() - 1) * 15]->originalLength);
	tailJoints[(dna->tail.getAmountOfRows() - 1) * 15 + 1]->setLength(-c1*sin(tailCounter / c3 + c4) + tailJoints[(dna->tail.getAmountOfRows() - 1) * 15 + 1]->originalLength);
	
	/*for (int j = 0; j < 4; j++)
	{
		tailJoints[j * 5]->setLength(0.3*cos((tailCounter) / 50.0) + tailJoints[j * 5]->originalLength);
		tailJoints[j * 5 + 1]->setLength(0.3*cos((tailCounter) / 50.0) + tailJoints[j * 5 + 1]->originalLength);
		tailJoints[j * 5 + 2]->setLength(-0.3*cos((tailCounter) / 50.0) + tailJoints[j * 5 + 2]->originalLength);
		tailJoints[j * 5 + 3]->setLength(-0.3*cos((tailCounter) / 50.0) + tailJoints[j * 5 + 3]->originalLength);
	}
	for (int j = 0; j < 4; j++)
	{
		tailJoints[20 + j * 5]->setLength(0.4*sin((tailCounter) / 50.0) + tailJoints[20 + j * 5]->originalLength);
		tailJoints[20 + j * 5 + 1]->setLength(0.4*sin((tailCounter) / 50.0) + tailJoints[20 + j * 5 + 1]->originalLength);
		tailJoints[20 + j * 5 + 2]->setLength(-0.4*sin((tailCounter) / 50.0) + tailJoints[20 + j * 5 + 2]->originalLength);
		tailJoints[20 + j * 5 + 3]->setLength(-0.4*sin((tailCounter) / 50.0) + tailJoints[20 + j * 5 + 3]->originalLength);
	}
	tailJoints[20 + 1 * 5 + 5]->setLength(0.8*sin((tailCounter) / 50.0) + tailJoints[20 + 1 * 5 + 5]->originalLength);
	tailJoints[20 + 1 * 5 + 6]->setLength(-0.8*sin((tailCounter) / 50.0) + tailJoints[20 + 1 * 5 + 6]->originalLength);*/
}

void Cell::movePoints(double precision, double backgroundFriction)
{
	float massPoint = getMass() / (1+amountEdges+tail.size());
	center->applyForces(precision, backgroundFriction, massPoint);
	for (auto point : edgePoints)
	{
		if (id == point->getBelongsTo()) {
			point->applyForces(precision, backgroundFriction, massPoint);
		}
	}
	for (auto point : tail)
	{
		if (id == point->getBelongsTo()) {
			point->applyForces(precision, backgroundFriction, massPoint);
		}
	}
}

void Cell::cellCellCollision(Cell* other)
{
	//calculate distance between 2 centers, if its shorter than the combined size, start detailed collision
	Vector connectedLine(center->getPlace(), other->center->getPlace());
	double distance = connectedLine.getLength();
	double combinedSize = radius + other->radius;
	if ((combinedSize * 2) > distance)
	{
		this->cellCellForce(other);
		other->cellCellForce(this);
	}
}

void Cell::cellCellForce(Cell* other)
{
	for (int i = 0; i < Cell::amountEdges; i++)
	{
		if (connectedCells[i] != other)
		{
			if (Vector::getLength(other->center->getPlace(), edgePoints[i]->getPlace()) < other->radius * 2) 
			{
				for (int j = 0; j < Cell::amountEdges; j++)
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

						double l1 = Vector::getLength(intersection, other->edgeJoints[j]->p1->getPlace());
						double l2 = Vector::getLength(intersection, other->edgeJoints[j]->p2->getPlace());

						other->edgeJoints[j]->p1->addForce(force*((-l2) / (l1 + l2)));
						other->edgeJoints[j]->p2->addForce(force*((-l1) / (l1 + l2)));

						//friction
						/*Vector otherVel = Vector::getAverage(other->edgeJoints[j]->p1->getVelocity(), other->edgeJoints[j]->p2->getVelocity());
						Vector velDif(otherVel, edgePoints[i]->getVelocity());
						
						velDif.multiply(world->c_CellCellFriction * edgePoints[i]->getMass() * overlap.getLength());

						other->edgeJoints[j]->p1->addForce(velDif*((l2) / (l1 + l2)));
						other->edgeJoints[j]->p2->addForce(velDif*((l1) / (l1 + l2)));

						double l1Own = Vector::getLength(intersection, center->getPlace());
						double l2Own = Vector::getLength(intersection, edgePoints[i]->getPlace());

						center->addForce       (velDif*((-l2Own) / (l1Own + l2Own)));
						edgePoints[i]->addForce(velDif*((-l1Own) / (l1Own + l2Own)));*/
					}
				}
			}
		}
	}
}

void Cell::lineCellCollision(Line* line){
	//check if its around the box of the line, otherwise dont check
	if (center->getPlace()->getX() + (radius * 3) > line->getLeft() && center->getPlace()->getX() - (radius * 3) < line->getRight() &&
		center->getPlace()->getY() + (radius * 3) > line->getTop() && center->getPlace()->getY() - (radius * 3) < line->getBottom())
	{
		//normal collision handeling
		lineCellForce(*line->getV1(), *line->getV2());
	}
}
void Cell::lineCellForce(Vector&perpendicular1, Vector&perpendicular2)
{
	for (int k = 0; k < Cell::amountEdges; k++)
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
			double moveDistance = Vector::getLength(&correctedIntersection, edgePoints[k]->getPlace());
			//c->edgePoints[k]->setPlace(correctedIntersection);

			//calculate the radius so you can calculate the angle between the perpendicular and the radius. Required to determain on which side of the line the center is
			Vector radius(center->getPlace(), edgePoints[k]->getPlace());
			radius.makeUnit();
			double angle = acos(radius.dot(perpendicular))*180.0 / 3.1415926535897;

			//calculate the normal force and add it
			if (abs(angle) < 90)
			{
				perpendicular.multiply(moveDistance * -world->c_LineCellCollisionForce * edgePoints[k]->getMass()*4);
			}
			else
			{
				perpendicular.multiply(moveDistance * world->c_LineCellCollisionForce * edgePoints[k]->getMass()*4);
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

void Cell::reRefPoints()
{
	center = center->getReal();
	for(int i =0;i<amountEdges;i++)
	{
		edgePoints[i] = edgePoints[i]->getReal();
	}
}

bool Cell::isBroken()const {
	//distance check
	for (int i = 0; i < amountEdges; i++) {
		if (Vector(center->getPlace(), edgePoints[i]->getPlace()).getLength() > radius * 3) {
			return true;
		}
	}
	//Edge cross check
	for (int i = 0; i < amountEdges; i++) {
		for (int j = 0; j < amountEdges-1; j++) {
			Vector intersection(0.0, 0.0);
			if (lineSegementsIntersect(*center->getPlace(),*edgePoints[i]->getPlace()
				,*edgePoints[j]->getPlace(), *edgePoints[j+1]->getPlace()
				,intersection,0.001)) {
				return true;
			}
		}
		Vector intersection(0.0, 0.0);
		if (lineSegementsIntersect(*center->getPlace(), *edgePoints[i]->getPlace()
			, *edgePoints[0]->getPlace(), *edgePoints[amountEdges - 1]->getPlace()
			, intersection, 0.001)) {
			return true;
		}
	}
	return false;
}

bool Cell::connectCells(Cell*other)
{
	int finalIndexOwn = 0;
	int finalIndexOther = 0;
	double distance = 10000;
	for(int i=0;i<amountEdges;i++)
	{
		for(int j=0;j<amountEdges;j++)
		{
			double d = pow(Vector::getLength(edgePoints[i]->getPlace(), other->edgePoints[j]->getPlace()),2);
			if(d<distance && connectedCells[i]==nullptr &&  other->connectedCells[j] == nullptr)
			{
				finalIndexOwn = i;
				finalIndexOther = j;
				distance = d;
			}
		}
	}
	if(distance<2)
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

		for (int l = 0; l < amountEdges; l++)
		{
			if (other->connectedCells[l] == this)
			{
				other->connectedCells[l] = nullptr;
			}
		}
	}
}

Membrane* Cell::getOuterMembrane() const
{
	return outerMembrane;
}

double Cell::getSurface()const
{
	return 2 * radius * 3.1415926535897;
}

int Cell::getId() const
{
	return id;
}

void Cell::applyForce(Vector& v)
{
	center->addForce(v / (amountEdges + 1.0));
	for (int i = 0; i < amountEdges; i++) {
		edgePoints[i]->addForce(v / (amountEdges + 1.0));
	}
}

shared_ptr<DNA> Cell::getDNA()
{
	dna->fitness = center->getPlace()->getX();
	return dna;
}

Vector* Cell::getEdgePoint(const int i)const
{
	return edgePoints[i]->getPlace();
}

Vector* Cell::getTailPoint(const int i, const int j) const
{
	if (j) 
	{
		return tailJoints[i]->getP1()->getPlace();
	}
	return tailJoints[i]->getP2()->getPlace();
}

int Cell::getEdgePointId(const int i) const
{
	return edgePoints[i]->getID();
}

shared_ptr<Point> Cell::getEdgePointPtr(const int i) const
{
	return edgePoints[i];
}

int Cell::getAmountOfEdgeEdges() const
{
	return amountEdges;
}

int Cell::getAmountOfTailEdges() const
{
	return tailJoints.size();
}

Joint* Cell::getEdgeEdge(int i) const
{
	return edgeJoints[i];
}

Joint* Cell::getTailEdge(int i) const
{
	return tailJoints[i];
}

Vector* Cell::getCenter()const
{
	return center->getPlace();
}

int Cell::getCenterId() const
{
	return center->getID();
}

shared_ptr<Point> Cell::getCenterPtr() const
{
	return center;
}

/*
void Cell::setSize(double t)
{
	double factor = t / size;
	size = t;
	multiplySize(factor);
}
double Cell::getSize()
{
	return size;
}
void Cell::multiplySize(double t)
{
	size *= t;
	center->multiplyMass(t);
	for (int i = 0; i < amountEdges; i++)
	{
		for (int j = 0; j < edgePoints[i]->getJointSize(); j++)
		{
			edgePoints[i]->getJoint(j)->setUnchanged();
		}
	}
	for (int i = 0; i < amountEdges; i++)
	{
		edgePoints[i]->multiplyMass(t);
		for (int j = 0; j < edgePoints[i]->getJointSize(); j++)
		{
			if (!edgePoints[i]->getJoint(j)->getChanged())
			{
				edgePoints[i]->getJoint(j)->multiplyLength(t);
				edgePoints[i]->getJoint(j)->setChanged();
			}
		}
	}
}*/
