#include "Joint.h"

Joint::Joint(shared_ptr<Point> tp1, shared_ptr<Point> tp2, double tStrength, double tDamping, bool tfriction, int tid, int tBelongsTo)
{
	p1 = tp1;
	p2 = tp2;
	p1->addJoint(this);
	p2->addJoint(this);
	length = Vector::getLength(tp1->getPlace(),tp2->getPlace());
	originalLength = length;
	strength = tStrength;
	damping = tDamping;
	friction = tfriction;
	id = tid;
	belongsTo = tBelongsTo;
}
Joint::~Joint()
{
	p1->deleteJointPointer(this);
	p2->deleteJointPointer(this);
}

void Joint::calcFriction(const Vector& flow)
{
	if (friction) 
	{
		frictionForce.set(0, 0);
		{
			Vector avgVelocity = Vector::getAverage(p1->getVelocity(), p2->getVelocity());
			Vector diffVelocity = Vector(p1->getVelocity(), p2->getVelocity());
			Vector base = Vector(p1->getPlace(), p2->getPlace()).getUnit();
			Vector antiNormal = base.getPerpendicularCounterClockwise();
			Vector flowPressure = avgVelocity - flow;
			Vector flowPressureUnit = -flowPressure.getUnit();
			double impactCooficient = antiNormal.dot(flowPressureUnit);
			double signCooficient = base.dot(flowPressureUnit);

			if (impactCooficient > 0)
			{
				Vector flowResponse = flowPressureUnit;

				Vector rotationCorrectedBase = base -(base.getUnit()*diffVelocity.getLength() * 4);
				if (signCooficient > 0)
				{
					flowResponse = Vector(antiNormal, impactCooficient, rotationCorrectedBase, 1 - impactCooficient).getUnit();
				}
				else
				{
					flowResponse = Vector(antiNormal, impactCooficient, -rotationCorrectedBase, 1 - impactCooficient).getUnit();
				}

				double length = Vector::getLength(p1->getPlace(), p2->getPlace());
				flowResponse.multiply(flowPressure.dot(flowPressure));
				flowResponse.multiply(0.3 * length * impactCooficient * 0.5);
				p1->addForce(flowResponse);
				p2->addForce(flowResponse);
				//frictionForce += flowResponse * 2; //For output
			}
		}
	}
	else
	{
		frictionForce.set(0, 0);
	}
}

shared_ptr<Point> Joint::getP1()
{
	return p1;
}

shared_ptr<Point> Joint::getP2()
{
	return p2;
}

shared_ptr<Point> Joint::getOther(shared_ptr<Point> p)
{
	if(p->getID()==p1->getID())
	{
		return p2;
	}else
	{
		return p1;
	}
}

double Joint::getLength()
{
	return length;
}

void Joint::setLength(double t)
{
	length = t;
}

void Joint::multiplyLength(double t)
{
	length *= t;
}

double Joint::getStrenth()
{
	return strength;
}

void Joint::setStrenth(double t)
{
	strength = t;
}

double Joint::getDamping()
{
	return damping;
}

void Joint::setDamping(double t)
{
	damping = t;
}

int Joint::getID()
{
	return id;
}

void Joint::setID(int t)
{
	id = t;
}

int Joint::getBelongsTo()
{
	return belongsTo;
}

void Joint::setBelongsTo(int t)
{
	belongsTo = t;
}

