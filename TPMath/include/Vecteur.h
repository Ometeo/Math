#include "./Point.h"

#ifndef VECTEUR_H
#define VECTEUR_H


class Vecteur
{
    private:
        Point _ptOrigin;
        Point _ptEnd;
        float _u;
        float _v;

    public:
        Vecteur(float u, float v);
        Vecteur(const Point pOrigin, const Point pEnd);
        virtual ~Vecteur();

        float GetU() const { return _u; }
        float GetV() const { return _v; }
        Point GetPtOrigin() const { return _ptOrigin; }
        Point GetPtEnd() const { return _ptEnd; }
        float DotProduct(const Vecteur &v) const;
        float Norm() const;
};

#endif // VECTEUR_H
