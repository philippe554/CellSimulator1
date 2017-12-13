#include "Vector.h"

Vector::Vector()
{
	x = 0; 
	y = 0;
}


Vector::Vector(const float tx, const float ty)
{
	x = tx;
	y = ty;
}

Vector::Vector(const Vector & v)
{
	x = v.x;
	y = v.y;
}

Vector::Vector(const Vector * v)
{
	x = v->x;
	y = v->y;
}

Vector::Vector(const Vector & v1, const Vector & v2)
{
	x = v2.x - v1.x;
	y = v2.y - v1.y;
}

Vector::Vector(const Vector * v1, const Vector * v2)
{
	x = v2->x - v1->x;
	y = v2->y - v1->y;
}

Vector::Vector(const Vector* v1, const float w1, const Vector* v2, const float w2)
{
	x = (v1->x*w1 + v2->x*w2) / (w1 + w2);
	y = (v1->y*w1 + v2->y*w2) / (w1 + w2);
}

Vector::Vector(const Vector& v1, const float w1, const Vector& v2, const float w2)
{
	x = (v1.x*w1 + v2.x*w2) / (w1 + w2);
	y = (v1.y*w1 + v2.y*w2) / (w1 + w2);
}

float Vector::getX()const
{
	return x;
}

float Vector::getY()const
{
	return y;
}

void Vector::setX(const float tx)
{
	x = tx;
}

void Vector::setY(const float ty)
{
	y = ty;
}

void Vector::add(const float tx, const float ty)
{
	x += tx;
	y += ty;
}

void Vector::multiply(const float t)
{
	x *= t;
	y *= t;
}

void Vector::devide(float t)
{
	if (t == 0)t = 0.001;
	x /= t;
	y /= t;
}

float Vector::getLength()const
{
	return sqrt(x*x + y*y);
}

float Vector::getLengthInvApprox() const
{
	float number = x*x + y*y;

	long i;
	float x2, y;
	const float threehalfs = 1.5F;

	x2 = number * 0.5F;
	y = number;
	i = *(long *)&y;                       // evil floating point bit level hacking
	i = 0x5f3759df - (i >> 1);               // what the fuck? 
	y = *(float *)&i;
	y = y * (threehalfs - (x2 * y * y));   // 1st iteration
	//	y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed

	return y;
}

bool Vector::isSmallerThen(float i)
{
	return x*x + y*y < i*i;
}

float Vector::getLength(const Vector* v1, const Vector* v2)
{
	return sqrt(((v1->x - v2->x)*(v1->x - v2->x)) + ((v1->y - v2->y)*(v1->y - v2->y)));
}

float Vector::getLength(const Vector & v1, const Vector & v2)
{
	return sqrt(((v1.x - v2.x)*(v1.x - v2.x)) + ((v1.y - v2.y)*(v1.y - v2.y)));
}

Vector Vector::getAverage(const Vector* v1, const Vector* v2)
{
	return Vector((v1->x + v2->x)/2, (v1->y + v2->y) / 2);
}

Vector Vector::getAverage(const Vector& v1, const Vector& v2)
{
	return Vector((v1.x + v2.x) / 2, (v1.y + v2.y) / 2);
}

float Vector::cross(const Vector * v)const
{
	return x * v->y - y * v->x;
}

float Vector::cross(const Vector & v)const
{
	return x * v.y - y * v.x;
}

Vector Vector::operator+(const Vector & v)const
{
	return Vector(x + v.x, y + v.y);
}

Vector Vector::operator+(const Vector * v)const
{
	return Vector(x + v->x, y + v->y);
}

void Vector::operator+=(const Vector& v)
{
	x += v.x;
	y += v.y;
}

Vector Vector::operator-(const Vector & v)const
{
	return Vector(x - v.x, y - v.y);
}

Vector Vector::operator-(const Vector * v)const
{
	return Vector(x - v->x, y - v->y);
}

void Vector::operator-=(const Vector& v)
{
	x -= v.x;
	y -= v.y;
}

Vector Vector::operator-() const
{
	return Vector(-x, -y);
}

Vector Vector::operator*(float r)const
{
	return Vector(x*r, y*r);
}

Vector Vector::operator/(float r)const
{
	if (r == 0)r = 0.001;
	return Vector(x / r, y / r);
}

float Vector::dot(const Vector & v)const
{
	return x*v.x + y*v.y;
}

float Vector::dot(const Vector * v)const
{
	return x*v->x + y*v->y;
}

void Vector::operator=(const Vector & v)
{
	x = v.x;
	y = v.y;
}

/*Vector Vector::getPerpendicular()const
{
	if (abs(x) < abs(y))
	{
		if (y == 0)y = 0.001;
		return Vector(1, -x / y);
	}
	else
	{
		if (x == 0)y = 0.001;
		return Vector(-y / x, 1);
	}
}*/

Vector Vector::getPerpendicularClockwise()const
{
	return Vector(y, -x);
}

Vector Vector::getPerpendicularCounterClockwise()const
{
	return Vector(-y, x);
}

Vector Vector::getUnit()const
{
	float length = getLength();
	if (length == 0)length = 0.001;
	return Vector(x / length, y / length);
}

void Vector::makeUnit()
{
	float length = getLength();
	if (length == 0)length = 0.001;
	x /= length;
	y /= length;
}

void Vector::putSquare()
{
	x *= x;
	y *= y;
}

void Vector::add(const Vector* v)
{
	x += v->x;
	y += v->y;
}

void Vector::add(const Vector & v)
{
	x += v.x;
	y += v.y;
}

void Vector::set(const Vector* v)
{
	x = v->x;
	y = v->y;
}

void Vector::set(const Vector & v)
{
	x = v.x;
	y = v.y;
}

void Vector::set(const float tx, const float ty)
{
	x = tx;
	y = ty;
}

void Vector::addX(const float tx)
{
	x += tx;
}

void Vector::addY(const float ty)
{
	y += ty;
}
