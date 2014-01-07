#include "../include/Point.h"

Point::~Point()
{
}

void Point::Translate(const Point &p)
{
    SetX(GetX() + p.GetX());
    SetY(GetY() + p.GetY());
}

Point Point::operator-(const Point& p)
{
    Point tmpPoint(GetX(), GetY());
    Point negPoint(-p.GetX(), -p.GetY());
    tmpPoint.Translate(negPoint);
    return tmpPoint;
}

Point Point::operator+(const Point& p)
{
    Point tmpPoint(GetX(), GetY());
	tmpPoint.Translate(p);
	return tmpPoint;
}
