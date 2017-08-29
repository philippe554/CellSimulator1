#include "Shapes.h"

bool Shapes::lineSegementsIntersect(const Vector&p, const Vector&p2, const Vector&q, const Vector&q2, Vector&intersection, double precision)
{
	Vector r = p2 - p;
	Vector s = q2 - q;
	double rxs = r.cross(s);
	if (rxs > 1e-5 || -rxs > 1e-5)
	{
		Vector p_q = q - p;
		double t = p_q.cross(s) / rxs;
		double u = p_q.cross(r) / rxs;
		if ((0 + precision < t && t < 1 - precision) && (0 + precision < u && u < 1 - precision))
		{
			intersection = p + (r*t);
			return true;
		}
	}
	return false;
}

bool Shapes::pointInTriangle(Vector & A, Vector & B, Vector & C, Vector & P)
{
	Vector v0 = C - A;
	Vector v1 = B - A;
	Vector v2 = P - A;

	double v0v0 = v0.dot(v0);
	double v0v1 = v0.dot(v1);
	double v0v2 = v0.dot(v2);
	double v1v1 = v1.dot(v1);
	double v1v2 = v1.dot(v2);

	double u = (v1v1*v0v2 - v0v1*v1v2) / (v0v0*v1v1 - v0v1*v0v1);
	double v = (v0v0*v1v2 - v0v1*v0v2) / (v0v0*v1v1 - v0v1*v0v1);
	if (u >= 0 && v >= 0 && u <= 1 && v <= 1 && (u + v) <= 1)
	{
		return true;
	}
	else
	{
		return false;
	}
}

float Shapes::surfaceTriangle(float l1, float l2, float l3)
{
	float s = 0.5*(l1+l2+l3);
	float t = s*(s - l1)*(s - l2)*(s - l3);
	if (t <= 0) return 0;
	return sqrt(t);
}

float Shapes::surfaceTriangle(const Vector & A, const Vector & B, const Vector & C)
{
	return surfaceTriangle(Vector::getLength(A, B), Vector::getLength(B, C), Vector::getLength(A, C));
}

