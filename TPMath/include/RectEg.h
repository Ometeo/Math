#include "./Point.h"
#include <vector>

using namespace std;

#ifndef RECTEG_H
#define RECTEG_H

class RectEg
{
    private:
        Point _botLeft;
        Point _topRight;

    public:
        RectEg(vector<Point> poly);
        virtual ~RectEg();
};

#endif // RECTEG_H
