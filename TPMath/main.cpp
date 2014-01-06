#include <windows.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <iostream>
#include <math.h>

#include "./include/Point.h"
#include "./include/Vecteur.h"

using namespace std;

double c;
bool drawPoly;
bool drawingNormals;

struct PointWithCnt {
    Point p;
    int nb;
};

vector<Point> windowsPoints;
vector<Point> polygonPoints;

vector<Point> polygonPointsWindowed;

vector<Vecteur> windowsNormals;

void init(int argc, char **argv);
bool isClockwise(vector<Point>);
void display();
void keyboard(unsigned char button, int x, int y);
void mouse(int mouseButton, int state, int x, int y);
void VectorNormal(vector<Point> polygonPoints);
void drawPoints(vector<Point> pointVector);
bool findPoint(int x, int y, vector<Point> pointVector, Point& p);
float VectorNorm(float x1, float x2, float y1, float y2);

void drawNormals(vector<Vecteur> normals);

// Ajoute ou retire le point p à la liste points
void addSpecialPoint(vector<PointWithCnt> &points, Point p);

//Algo sutherland-Hodgman
void SutherlandHodgman(vector<Point> polygonPoints, vector<Point> windowsPoints);


//Retourne un booléen suivant l'intersection possible entre le côté [currentPolyPoint otherPolyPoint] du polygone et le bord prolongé (une droite) (windowPoint1 windowPoint2) de la fenêtre.
bool cut(float t);

//retournant le point d'intersection [[currentPolyPoint otherPolyPoint] inter (windowPoint1 windowPoint2)
Point intersect(float t, Point currentPolyPoint, Point otherPolyPoint);

//Retourne un booléen si S est visible par rapport à (windowPoint1 windowPoint2
bool visible(Vecteur normal,Point currentPolyPoint, Point windowPoint1, Point windowPoint2);

float calcTforIntersect(Vecteur normal, Point currentPolyPoint, Point otherPolyPoint, Point windowPoint1, Point windowPoint2);


int main (int argc, char **argv)
{

    init(argc, argv);

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);

    glutMainLoop();
    return 0;
}

void init(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("TP Math : Fenetrage & Remplissage");

    gluOrtho2D(-250.0, 250.0, -250.0, 250.0);

    glClearColor(0.0,0.0,0.0,0.0);
    glColor3f(1.0,1.0,1.0);
    glPointSize(2.0);

    drawPoly = true;
    drawingNormals = false;
}

/**
* This function returns true if [poly] is clockwise
*/
bool isClockwise(vector<Point> poly)
{
    int sum = 0;
    for(unsigned int i = 0; i < poly.size(); i++)
    {
        int x1 = poly[i].GetX();
        int y1 = poly[i].GetY();
        int x2 = poly[(i + 1) % poly.size()].GetX();
        int y2 = poly[(i + 1) % poly.size()].GetY();

        sum += (x2-x1)*(y2+y1);
    }
    return sum > 0;
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(1.0, 0.0, 0.0);
    drawPoints(windowsPoints);

    if(windowsPoints.size() > 1)
    {
        for(unsigned int i = 0; i < windowsPoints.size(); i++)
        {
            glBegin(GL_LINES);
            glVertex2i(windowsPoints[i].GetX(), windowsPoints[i].GetY());
            glVertex2i(windowsPoints[(i + 1) % windowsPoints.size()].GetX(), windowsPoints[(i + 1) % windowsPoints.size()].GetY());
            glEnd();
        }
    }

    glColor3f(0.0, 1.0, 0.0);
    drawPoints(polygonPoints);
    if(polygonPoints.size() > 1)
    {
        Point pt1, pt2;
        for(unsigned int i = 0; i < polygonPoints.size(); i++)
        {
            pt1 = polygonPoints[i];
            pt2 = polygonPoints[(i + 1) % polygonPoints.size()];
            glBegin(GL_LINES);
            glVertex2i(pt1.GetX(), pt1.GetY());
            glVertex2i(pt2.GetX(), pt2.GetY());
            glEnd();
        }
    }
    if(drawingNormals)
        drawNormals(windowsNormals);

    glColor3f(1.0,1.0,1.0);
    drawPoints(polygonPointsWindowed);
    if(polygonPointsWindowed.size() > 1)
    {
        Point pt1, pt2;

        cout << "size : " << polygonPointsWindowed.size() << endl;

        for(unsigned int i = 0; i < polygonPointsWindowed.size(); i++)
        {
            pt1 = polygonPointsWindowed[i];
            pt2 = polygonPointsWindowed[(i + 1) % polygonPointsWindowed.size()];
            glBegin(GL_LINES);
            glVertex2i(pt1.GetX(), pt1.GetY());
            glVertex2i(pt2.GetX(), pt2.GetY());
            glEnd();
        }
    }




//    cout << "polygon isClockwise : " << isClockwise(windowsPoints) << endl;

    glFlush();
}

void mouse(int button,int state,int x,int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        Point temp(x - 250, -y + 250);
        if(drawPoly)
            polygonPoints.push_back(temp);
        else
        {
            windowsPoints.push_back(temp);
            if(windowsPoints.size() > 2)
            {
                VectorNormal(windowsPoints);
            }
        }

        if(polygonPoints.size() > 1 && windowsPoints.size() > 2)
        {
            SutherlandHodgman(polygonPoints, windowsPoints);
        }


        display();
    }

    if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
    {
        if(drawPoly)
        {
            Point temp;
            bool found = findPoint(x - 250, -y + 250, polygonPoints, temp);
            if(!found)
            {
                polygonPoints.push_back(temp);
            }
        }
    }
}

void keyboard(unsigned char touche,int x,int y)
{
    switch (touche)
    {
    case 'w':
        drawPoly = false;
        break;
    case 'p':
        drawPoly = true;
        break;
    case 'n':
        drawingNormals = !drawingNormals;
        display();
        break;


//		case 'p':/* affichage du carré plein*/
//			glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
//			glutPostRedisplay();     // permet l'affichage
//			break;

//		case 'f':/* affichage en mode fil de fer*/
//			glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
//			glutPostRedisplay();
//			break;

//		case 's':/* affichage en mode sommets seuls*/
//			glPolygonMode(GL_FRONT_AND_BACK,GL_Point);
//			glutPostRedisplay();
//			break;

    case 'q':/* Quitter le programme */
        exit(0);
    }
}


void drawPoints(vector<Point> pointVector)
{
    glBegin(GL_POINTS);
    for(unsigned int i = 0; i < pointVector.size(); i++)
    {
        glVertex2i(pointVector[i].GetX(), pointVector[i].GetY());
    }
    glEnd();
}

void drawNormals(vector<Vecteur> normals)
{
    glColor3f(1.0,1.0,0.0);
    for(unsigned int i = 0; i < normals.size(); i++)
    {
        glBegin(GL_POINTS);
        glVertex2i(normals[i].GetPtOrigin().GetX(), normals[i].GetPtOrigin().GetY());
        glVertex2i(normals[i].GetPtEnd().GetX(), normals[i].GetPtEnd().GetY());
        glEnd();

        glBegin(GL_LINES);
        glVertex2i(normals[i].GetPtOrigin().GetX(), normals[i].GetPtOrigin().GetY());
        glVertex2i(normals[i].GetPtEnd().GetX(), normals[i].GetPtEnd().GetY());
        glEnd();
    }
}


//void sortPointsByDistance(vector<Point> &points)
//{
//    for (int currentP = 0; currentP < points.size() - 1; currentP++)
//    {
//        int min = 99999999;
//        int indexMin = 0;
//        for (int nextP = currentP + 1; nextP < points.size(); nextP++)
//        {
//            Vecteur pi_pi1(points[currentP], points[nextP]);
//            float norm = pi_pi1.Norm();
//
//            if (norm < min)
//            {
//                min = norm;
//                indexMin = nextP;
//            }
//        }
//        Point tmp = points[currentP + 1];
//        points[currentP + 1] = points[indexMin];
//        points[indexMin] = tmp;
//    }
//}

bool findPoint(int x, int y, vector<Point> pointVector, Point& p)
{
    return false;
}

void VectorNormal(vector<Point> polygonPoints)
{
    bool clockwise = isClockwise(polygonPoints);
    float X, Y, vectorNormalY, vectorNormalX;

    float norm;

    Point pt1, pt2;

    windowsNormals.clear();

    for(unsigned int i = 0; i < polygonPoints.size(); i++)
    {
        pt1 = polygonPoints[i];
        pt2 = polygonPoints[(i + 1) % polygonPoints.size()];

        Y = pt1.GetY() + (pt2.GetY()- pt1.GetY())*0.5;
        X = pt1.GetX() + (pt2.GetX()- pt1.GetX())*0.5;

        if(clockwise)
        {
            vectorNormalX = pt2.GetY() - pt1.GetY();
            vectorNormalY = -(pt2.GetX()-pt1.GetX());
        }
        else
        {
            vectorNormalX = - (pt2.GetY() - pt1.GetY());
            vectorNormalY =  pt2.GetX() - pt1.GetX();
        }

        Point pA = Point(X, Y);
        norm = Vecteur(Point(vectorNormalX, vectorNormalY), pA).Norm();
        vectorNormalX /= norm / 10;
        vectorNormalY /= norm / 10;

        Point pB = Point(X + vectorNormalX, Y + vectorNormalY);

        Vecteur v(pA, pB);

        windowsNormals.push_back(v);
    }
}

//Algo sutherland-Hodgman
void SutherlandHodgman(vector<Point> polygonPoints, vector<Point> windowsPoints)
{
    vector<Point> PS;
    vector<PointWithCnt> polyPointsInside;

    Point F;
    Point S;
    Point I;

    cout << "algo stherland hodgman " << endl;
    polygonPointsWindowed.clear();
    polygonPointsWindowed = polygonPoints;
    for(unsigned int i = 0; i < windowsPoints.size(); i++)
    {
        PS.clear();
        for(unsigned int j = 0; j < polygonPointsWindowed.size(); j++)
        {
            cout << "i : " << i << ", j : " << j << endl;
            if(j == 0)
            {
                F = polygonPointsWindowed[j];
            }
            else
            {
                float t = calcTforIntersect(windowsNormals[i], S, polygonPointsWindowed[j], windowsPoints[i], windowsPoints[(i + 1) % windowsPoints.size()]);
                if(cut(t))
                {
                    cout << "cut" << endl;
                    I = intersect(t, S, polygonPointsWindowed[j]);
                    PS.push_back(I);
                }
            }
            S = polygonPointsWindowed[j];
            if(visible(windowsNormals[i], S, windowsPoints[i], windowsPoints[(i + 1) % windowsPoints.size()]))
            {
                PS.push_back(S);
            }
        }
        if(PS.size() > 0)
        {
            float t = calcTforIntersect(windowsNormals[i], S, F, windowsPoints[i], windowsPoints[(i + 1) % windowsPoints.size()]);
            if(cut(t))
            {
                I = intersect(t, S, F);
                PS.push_back(I);
            }
        }
        polygonPointsWindowed = PS;
    }
}

void addSpecialPoint(vector<PointWithCnt> &points, Point p)
{
    bool found = false;
    for (int i = 0; i < points.size(); i++)
    {
        if (p.GetX() == points[i].p.GetX() && p.GetY() == points[i].p.GetY())
        {
            found = true;
            points[i].nb += 1;
            break;
        }
    }
    if (!found)
    {
        PointWithCnt newP;
        newP.p = p;
        newP.nb = 1;
        points.push_back(newP);
    }
}

//Retourne un booléen suivant l'intersection possible entre le côté [currentPolyPoint otherPolyPoint] du polygone et le bord prolongé (une droite) (windowPoint1 windowPoint2) de la fenêtre.
bool cut(float t)
{
    if (t >= 0 && t <= 1)
        return true;
    else
        return false;
}

float calcTforIntersect(Vecteur ni, Point A, Point B, Point Pi, Point Pi1)
{
    Vecteur D = Vecteur(A, B);

    Vecteur W = Vecteur(Pi, A);

    float t = -1;
    float DdotNi = D.DotProduct(ni);
    if (DdotNi != 0)
        t = - W.DotProduct(ni) / DdotNi;
    return t;
}


Point intersect(float t, Point A, Point B)
{
    // [AB] : Q(t) = (1 - t)A + tB, 0 <= t <= 1

    Point I;
    Point tmpA = Point((1 - t) * A.GetX(), (1 - t) * A.GetY());
    Point tmpB = Point(t * B.GetX(), t * B.GetY());

    I = tmpA + tmpB;

    return I;
}


//Retourne un booléen si S est visible par rapport à (windowPoint1 windowPoint2

bool visible(Vecteur ni, Point currentPolyPoint, Point windowPoint1, Point windowPoint2)
{
    Vecteur FiPi(windowPoint1, currentPolyPoint);

    if(0 <= FiPi.DotProduct(ni))
        return true;
    else
        return false;
}



