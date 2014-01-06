#include <math.h>
#include "../include/Vecteur.h"
#include "../include/Point.h"

Vecteur::Vecteur(float u, float v)
{
    _u = u;
    _v = v;
}

Vecteur::Vecteur(const Point pOrigin, const Point pEnd)
{
    _ptOrigin = Point(pOrigin.GetX(), pOrigin.GetY());
    _ptEnd = Point(pEnd.GetX(), pEnd.GetY());
    _u = pEnd.GetX() - pOrigin.GetX();
    _v = pEnd.GetY() - pOrigin.GetY();
}

Vecteur::~Vecteur()
{
    //dtor
}

float Vecteur::Norm() const
{
    return sqrt(GetU()* GetU() + GetV() * GetV());
}

float Vecteur::DotProduct(const Vecteur &v) const
{
    return GetU() * v.GetU() + GetV() * v.GetV();
}
