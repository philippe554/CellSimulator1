#pragma once

class Vector;

#include <algorithm>

class Vector {
public:
	Vector();
	Vector(const float tx, const float ty);
	Vector(const Vector&v);
	Vector(const Vector*v);
	Vector(const Vector&v1, const Vector&v2);
	Vector(const Vector*v1, const Vector*v2);
	Vector(const Vector*v1, const float w1, const Vector*v2, const float w2);
	Vector(const Vector&v1, const float w1, const Vector&v2, const float w2);

	float getX() const;
	float getY() const;

	void setX(const float tx);
	void setY(const float ty);
	void set(const Vector* v);
	void set(const Vector& v);
	void set(const float tx, const float ty);

	void addX(const float tx);
	void addY(const float ty);
	void add(const Vector* v);
	void add(const Vector& v);
	void add(const float tx, const float ty);

	void multiply(const float t);
	void devide(float t);

	float getLength() const;
	float getLengthInvApprox() const;
	bool isSmallerThenSquared(float i);
	static float getLength(const Vector* v1, const Vector* v2);
	static float getLength(const Vector& v1, const Vector& v2);

	static Vector getAverage(const Vector* v1, const Vector* v2);
	static Vector getAverage(const Vector& v1, const Vector& v2);

	float cross(const Vector* v)const;
	float cross(const Vector& v)const;

	Vector operator+(const Vector&v)const;
	Vector operator+(const Vector*v)const;
	void operator+=(const Vector& v);

	Vector operator-(const Vector&v)const;
	Vector operator-(const Vector*v)const;
	void operator-=(const Vector& v);
	Vector operator-()const;

	Vector operator*(const float r)const;
	Vector operator/(const float r)const;

	float dot(const Vector&v)const;
	float dot(const Vector*v)const;

	void operator=(const Vector&v);

	//Vector getPerpendicular()const;
	Vector getPerpendicularClockwise()const;
	Vector getPerpendicularCounterClockwise()const;

	Vector getUnit()const;
	Vector makeUnit();

	void putSquare();

private:
	float x;
	float y;
};
