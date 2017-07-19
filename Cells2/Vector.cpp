#include "Vector.h"

Vector::Vector()
{
	x = 0; 
	y = 0;
}


Vector::Vector(const double tx, const double ty)
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

Vector::Vector(const Vector* v1, const double w1, const Vector* v2, const double w2)
{
	x = (v1->x*w1 + v2->x*w2) / (w1 + w2);
	y = (v1->y*w1 + v2->y*w2) / (w1 + w2);
}

Vector::Vector(const Vector& v1, const double w1, const Vector& v2, const double w2)
{
	x = (v1.x*w1 + v2.x*w2) / (w1 + w2);
	y = (v1.y*w1 + v2.y*w2) / (w1 + w2);
}

double Vector::getX()const
{
	return x;
}

double Vector::getY()const
{
	return y;
}

void Vector::setX(const double tx)
{
	x = tx;
}

void Vector::setY(const double ty)
{
	y = ty;
}

void Vector::add(const double tx, const double ty)
{
	x += tx;
	y += ty;
}

void Vector::multiply(const double t)
{
	x *= t;
	y *= t;
}

void Vector::devide(double t)
{
	if (t == 0)t = 0.001;
	x /= t;
	y /= t;
}

double Vector::getLength()const
{
	return sqrt(x*x + y*y);
}

double Vector::getLength(const Vector* v1, const Vector* v2)
{
	return sqrt(((v1->x - v2->x)*(v1->x - v2->x)) + ((v1->y - v2->y)*(v1->y - v2->y)));
}

double Vector::getLength(const Vector & v1, const Vector & v2)
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

double Vector::cross(const Vector * v)const
{
	return x * v->y - y * v->x;
}

double Vector::cross(const Vector & v)const
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

Vector Vector::operator*(double r)
{
	return Vector(x*r, y*r);
}

Vector Vector::operator/(double r)
{
	if (r == 0)r = 0.001;
	return Vector(x / r, y / r);
}

double Vector::dot(const Vector & v)const
{
	return x*v.x + y*v.y;
}

double Vector::dot(const Vector * v)const
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
	double length = getLength();
	if (length == 0)length = 0.001;
	return Vector(x / length, y / length);
}

Vector Vector::makeUnit()
{
	double length = getLength();
	if (length == 0)length = 0.001;
	x /= length;
	y /= length;
	return *this;
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

void Vector::set(const double tx, const double ty)
{
	x = tx;
	y = ty;
}

void Vector::addX(const double tx)
{
	x += tx;
}

void Vector::addY(const double ty)
{
	y += ty;
}
