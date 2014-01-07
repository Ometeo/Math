#include "../include/RectEg.h"
#include "../include/Point.h"
#include <vector>

using namespace std;

RectEg::RectEg(vector<Point> poly)
{
    int xMin = 999999;
    int yMin = 999999;
    int xMax = -999999;
    int yMax = -999999;
    for(int numPoint = 0; numPoint < poly.size(); numPoint++)
    {
        if(poly[numPoint].GetX() < xMin)
            xMin = poly[numPoint].GetX();
        else if (poly[numPoint].GetX() > xMax)
            xMax = poly[numPoint].GetX();
        if(poly[numPoint].GetY() < yMin)
            yMin = poly[numPoint].GetY();
        else if (poly[numPoint].GetY() > yMax)
            yMax = poly[numPoint].GetY();
    }
    _botLeft.SetCoord(xMin, yMin);
    _topRight.SetCoord(xMax, yMax);
}

RectEg::~RectEg()
{
    //dtor
}
