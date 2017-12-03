#include "Joint.h"
#include "Shapes.h"

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
	for (int i = 0; i < WorldSettings::e_AmountOfParticles; i++)
	{
		flowRate[i] = 0.5;
	}
	//friction = 0;
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
	for (int i = 0; i < WorldSettings::e_AmountOfParticles; i++)
	{
		flowRate[i] = 0.5;
	}
	//friction = tfriction;
	id = lastID;
	lastID++;
}

void Joint::init(Point* tp1, Point* tp2, const Joint& other)
{
	active = true;
	p1 = tp1;
	p2 = tp2;
	p1->addJoint(this);
	p2->addJoint(this);
	length = other.length;
	targetLength = other.targetLength;
	growFactor = other.growFactor;
	strength = other.strength;
	damping = other.damping;
	for (int i = 0; i < WorldSettings::e_AmountOfParticles; i++)
	{
		flowRate[i] = other.flowRate[i];
	}
	//friction = other.friction;
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
	//friction = 0;
}

bool Joint::changeFromTo(Point * from, Point * to)
{
	if (from->getID() == p1->getID())
	{
		p1 = to;
		return true;
	}
	else if (from->getID() == p2->getID())
	{
		p2 = to;
		return true;
	}
	else
	{
		return false;
	}
}

/*Vector Joint::calcFriction(const Vector& flow)
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
			flowResponse.multiply(length * impactCooficient * 0.5 * 0.3);
			p1->addForce(flowResponse);
			p2->addForce(flowResponse);
			flowResponse.multiply(2);
			//frictionForce = flowResponse;
			return flowResponse;
		}
	}
	else
	{
		frictionForce.set(0, 0);
	}
	return Vector(0.0, 0.0);
}
*/
/*void Joint::applyPresureForce(float p)
{
	Vector jointLine = Vector(p1->getPlace(), p2->getPlace());
	float surface = jointLine.getLength();
	Vector normal = jointLine.getPerpendicularCounterClockwise().getUnit();
	normal.multiply(0.5*surface*p);
	p1->addForce(normal);
	p2->addForce(normal);
}*/

void Joint::jointJointCollision(Joint* other)
{
	throw "Old Code";
	Vector intersection(0.0, 0.0);
	if (Shapes::lineSegementsIntersect(p1->getPlace(), p2->getPlace(), other->p1->getPlace(), other->p2->getPlace(), intersection))
	{
		Vector jointLine1 = Vector(p1->getPlace(), p2->getPlace());
		Vector normal1 = jointLine1.getPerpendicularCounterClockwise().getUnit();
		Vector jointLine2 = Vector(other->p1->getPlace(), other->p2->getPlace());
		Vector normal2 = jointLine2.getPerpendicularCounterClockwise().getUnit();
		float size = Vector::getLength(p1->getPlace(), other->p2->getPlace()) + Vector::getLength(other->p1->getPlace(), p2->getPlace());
		Vector force1 = Vector::getAverage(normal1, -normal2).getUnit();
		force1.multiply(size * 0.5);
		Vector force2 = -force1;

		float j1l1 = Vector::getLength(p1->getPlace(), intersection);
		float j1l2 = Vector::getLength(p2->getPlace(), intersection);
		float j2l1 = Vector::getLength(other->p1->getPlace(), intersection);
		float j2l2 = Vector::getLength(other->p2->getPlace(), intersection);

		//p1->addForce(force1*((j1l2) / (j1l1 + j1l2)));
		//p2->addForce(force1*((j1l1) / (j1l1 + j1l2)));

		//other->p1->addForce(force2*((j2l2) / (j2l1 + j2l2)));
		//other->p2->addForce(force2*((j2l1) / (j2l1 + j2l2)));
	}
}

/*void Joint::setFriction(bool _friction)
{
	friction = _friction;
}*/

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

void Joint::logic(const float precision)
{
	if (growFactor > 0 && targetLength != length)
	{
		float gf = growFactor * precision;
		if (targetLength > length)
		{
			if (targetLength - length <= gf)
			{
				length = targetLength;
			}
			else
			{
				length += gf;
			}
		}
		else
		{
			if (length - targetLength <= gf)
			{
				length = targetLength;
			}
			else
			{
				length -= gf;
			}
		}
	}

	for (int i = 0; i < WorldSettings::e_AmountOfParticles; i++)
	{
		float m1 = p1->getParticleCount(i);
		float m2 = p2->getParticleCount(i);
		float ratio = m1 / (m1+m2);
		float flow = (flowRate[i] - ratio)*0.01;
		if(flow < 0.0)
		{
			p1->moveParticle(p2, i, m1 * -flow);
		}
		else
		{
			p2->moveParticle(p1, i, m2 * flow);
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

float Joint::getFlowRate(int i) const
{
	return flowRate[i];
}

void Joint::setFlowRate(const int i, const float t)
{
	flowRate[i] = t;
}

/*const Vector & Joint::getFrictionForce()const
{
	return frictionForce;
}*/

long Joint::getId()const
{
	return id;
}