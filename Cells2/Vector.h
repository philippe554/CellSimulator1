#pragma once

class Vector;

#include <algorithm>

class Vector {
public:
	Vector();
	Vector(const double tx, const double ty);
	Vector(const Vector&v);
	Vector(const Vector*v);
	Vector(const Vector&v1, const Vector&v2);
	Vector(const Vector*v1, const Vector*v2);
	Vector(const Vector*v1, const double w1, const Vector*v2, const double w2);
	Vector(const Vector&v1, const double w1, const Vector&v2, const double w2);

	double getX() const;
	double getY() const;

	void setX(const double tx);
	void setY(const double ty);
	void set(const Vector* v);
	void set(const Vector& v);
	void set(const double tx, const double ty);

	void addX(const double tx);
	void addY(const double ty);
	void add(const Vector* v);
	void add(const Vector& v);
	void add(const double tx, const double ty);

	void multiply(const double t);
	void devide(double t);

	double getLength() const;
	static double getLength(const Vector* v1, const Vector* v2);
	static double getLength(const Vector& v1, const Vector& v2);

	static Vector getAverage(const Vector* v1, const Vector* v2);
	static Vector getAverage(const Vector& v1, const Vector& v2);

	double cross(const Vector* v)const;
	double cross(const Vector& v)const;

	Vector operator+(const Vector&v)const;
	Vector operator+(const Vector*v)const;
	void operator+=(const Vector& v);

	Vector operator-(const Vector&v)const;
	Vector operator-(const Vector*v)const;
	void operator-=(const Vector& v);
	Vector operator-()const;

	Vector operator*(const double r);
	Vector operator/(const double r);

	double dot(const Vector&v)const;
	double dot(const Vector*v)const;

	void operator=(const Vector&v);

	//Vector getPerpendicular()const;
	Vector getPerpendicularClockwise()const;
	Vector getPerpendicularCounterClockwise()const;

	Vector getUnit()const;
	Vector makeUnit();

	void putSquare();

private:
	double x;
	double y;
};
