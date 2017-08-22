#include "Joint.h"

long Joint::lastID = 0;

Joint::Joint() 
{
	active = false;
	p1 = nullptr;
	p1 = nullptr;
	length = 0;
	targetLength = 0;
	growFactor = 0;
	strength = 0;
	damping = 0;
	friction = 0;
}

void Joint::init(Point* tp1, Point* tp2, const float tStrength, const float tDamping, const bool tfriction)
{
	active = true;
	p1 = tp1;
	p2 = tp2;
	p1->addJoint(this);
	p2->addJoint(this);
	length = Vector::getLength(tp1->getPlace(),tp2->getPlace());
	targetLength = length;
	growFactor = 0;
	strength = tStrength;
	damping = tDamping;
	friction = tfriction;
	id = lastID;
	lastID++;
}

void Joint::deconstruct()
{
	active = false;
	p1->deleteJoint(id);
	p2->deleteJoint(id);
	p1 = nullptr;
	p1 = nullptr;
	length = 0;
	targetLength = 0;
	growFactor = 0;
	strength = 0;
	damping = 0;
	friction = 0;
}

Vector Joint::calcFriction(const Vector& flow)
{
	if (friction) 
{
		Vector avgVelocity = Vector::getAverage(p1->getVelocity(), p2->getVelocity());
		Vector diffVelocity = Vector(p1->getVelocity(), p2->getVelocity());
		Vector base = Vector(p1->getPlace(), p2->getPlace()).getUnit();
		Vector antiNormal = base.getPerpendicularCounterClockwise();
		Vector flowPressure = avgVelocity - flow;
		Vector flowPressureUnit = -flowPressure.getUnit();
		float impactCooficient = antiNormal.dot(flowPressureUnit);
		float signCooficient = base.dot(flowPressureUnit);

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

			float length = Vector::getLength(p1->getPlace(), p2->getPlace());
			flowResponse.multiply(flowPressure.dot(flowPressure));
			flowResponse.multiply(0.3 * length * impactCooficient * 0.5);
			p1->addForce(flowResponse);
			p2->addForce(flowResponse);
			flowResponse.multiply(2);
			frictionForce = flowResponse;
			return flowResponse;
		}
	}
	else
	{
		frictionForce.set(0, 0);
	}
	return Vector(0.0, 0.0);
}

Point* Joint::getP1()const
{
	return p1;
}

Point* Joint::getP2()const
{
	return p2;
}

Point* Joint::getOther(Point* p)const
{
	if(p->getID()==p1->getID())
	{
		return p2;
	}else
	{
		return p1;
	}
}

float Joint::getLength()
{
	return length;
}

void Joint::setLength(float t)
{
	length = t;
}

float Joint::getRealLength()const
{
	return Vector::getLength(p1->getPlace(), p2->getPlace());
}

void Joint::setTarget(const float _targetLength, const float _growFactor)
{
	targetLength = _targetLength;
	growFactor = _growFactor;
}

bool Joint::grow(const float precision)
{
	if (growFactor > 0 && targetLength != length)
	{
		float gf = growFactor * precision;
		if (targetLength > length)
		{
			if (targetLength - length <= gf)
			{
				length = targetLength;
				return true;
			}
			else
			{
				length += gf;
				return false;
			}
		}
		else
		{
			if (length - targetLength <= gf)
			{
				length = targetLength;
				return true;
			}
			else
			{
				length -= gf;
				return false;
			}
		}
	}
}

float Joint::getStrenth()
{
	return strength;
}

void Joint::setStrenth(float t)
{
	strength = t;
}

float Joint::getDamping()
{
	return damping;
}

void Joint::setDamping(float t)
{
	damping = t;
}

const Vector & Joint::getFrictionForce()const
{
	return frictionForce;
}

long Joint::getId()const
{
	return id;
}