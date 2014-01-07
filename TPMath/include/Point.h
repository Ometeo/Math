#ifndef POINT_H
#define POINT_H


class Point
{
    private:
        float _x;
        float _y;

    public:
        Point(){ _x = 0; _y = 0; }
        Point(float x, float y) { _x = x; _y = y; }
        Point(const Point &p) { _x = p.GetX(); _y = p.GetY(); }
        virtual ~Point();
        float GetX() const { return _x; }
        float GetY() const { return _y; }
        void SetX(float x) { _x = x; }
        void SetY(float y) { _y = y; }
        void SetCoord(int x, int y) { SetX(x); SetY(y); }

        void Translate(const Point &p);
        Point operator-(const Point& p);
        Point operator+(const Point& p);
};

#endif // POINT_H
