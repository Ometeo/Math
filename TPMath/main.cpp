#include <windows.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <iostream>
#include <math.h>

using namespace std;

double c;
bool drawPoly;
bool drawingNormals;


struct vec {
    float x1;
    float y1;
    float x2;
    float y2;
};

struct vect {
    float x;
    float y;
};

struct PointWithCnt {
    POINT p;
    int nb;
};

vector<POINT> windowsPoints;
vector<POINT> polygonPoints;

vector<POINT> polygonPointsWindowed;

vector<vec> windowsNormals;

void init(int argc, char **argv);
bool isClockwise(vector<POINT>);
void display();
void keyboard(unsigned char button, int x, int y);
void mouse(int mouseButton, int state, int x, int y);
void VectorNormal(vector<POINT> polygonPoints);
void drawPoints(vector<POINT> pointVector);
bool findPoint(int x, int y, vector<POINT> pointVector, POINT& p);
float VectorNorm(float x1, float x2, float y1, float y2);

float DotProduct(vect v1, vect v2);
float DotProduct(float x1, float x2, float y1, float y2);
void drawNormals(vector<vec> normals);

// Ajoute ou retire le point p à la liste points
void addSpecialPoint(vector<PointWithCnt> &points, POINT p);

//Algo sutherland-Hodgman
void SutherlandHodgman(vector<POINT> polygonPoints, vector<POINT> windowsPoints);


//Retourne un booléen suivant l'intersection possible entre le côté [currentPolyPoint otherPolyPoint] du polygone et le bord prolongé (une droite) (windowPoint1 windowPoint2) de la fenêtre.
bool cut(vec normal, POINT currentPolyPoint, POINT otherPolyPoint, POINT windowPoint1, POINT windowPoint2);

//retournant le point d'intersection [[currentPolyPoint otherPolyPoint] inter (windowPoint1 windowPoint2)
POINT intersect(vec normal, POINT currentPolyPoint, POINT otherPolyPoint, POINT windowPoint1, POINT windowPoint2);

//Retourne un booléen si S est visible par rapport à (windowPoint1 windowPoint2
bool visible(vec normal,POINT currentPolyPoint, POINT windowPoint1, POINT windowPoint2);

float calcTforIntersect(vec normal, POINT currentPolyPoint, POINT otherPolyPoint, POINT windowPoint1, POINT windowPoint2);



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
bool isClockwise(vector<POINT> poly)
{
    int sum = 0;
    for(unsigned int i = 0; i < poly.size(); i++)
    {
        int x1 = poly[i].x;
        int y1 = poly[i].y;
        int x2 = poly[(i + 1) % poly.size()].x;
        int y2 = poly[(i + 1) % poly.size()].y;

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
        for(unsigned int i = 1; i < windowsPoints.size(); i++)
        {
            glBegin(GL_LINES);
            glVertex2i(windowsPoints[i-1].x, windowsPoints[i-1].y);
            glVertex2i(windowsPoints[i].x, windowsPoints[i].y);
            glEnd();
        }
        glBegin(GL_LINES);
        glVertex2i(windowsPoints[windowsPoints.size()-1].x, windowsPoints[windowsPoints.size()-1].y);
        glVertex2i(windowsPoints[0].x, windowsPoints[0].y);
        glEnd();
    }

    glColor3f(0.0, 1.0, 0.0);
    drawPoints(polygonPoints);
    if(polygonPoints.size() > 1)
    {
        POINT pt1, pt2;
        for(unsigned int i = 0; i < polygonPoints.size(); i++)
        {
            pt1 = polygonPoints[i];
            pt2 = polygonPoints[(i + 1) % polygonPoints.size()];
            glBegin(GL_LINES);
            glVertex2i(pt1.x, pt1.y);
            glVertex2i(pt2.x, pt2.y);
            glEnd();
        }
    }
    if(drawingNormals)
        drawNormals(windowsNormals);

    glColor3f(1.0,1.0,1.0);
    drawPoints(polygonPointsWindowed);
    if(polygonPointsWindowed.size() > 1)
    {
        POINT pt1, pt2;

        cout << "size : " << polygonPointsWindowed.size() << endl;

        for(unsigned int i = 0; i < polygonPointsWindowed.size(); i++)
        {
            pt1 = polygonPointsWindowed[i];
            pt2 = polygonPointsWindowed[(i + 1) % polygonPointsWindowed.size()];
            glBegin(GL_LINES);
            glVertex2i(pt1.x, pt1.y);
            glVertex2i(pt2.x, pt2.y);
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
        POINT temp;
        temp.x = x - 250;
        temp.y = -y + 250;
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
            POINT temp;
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
//			glPolygonMode(GL_FRONT_AND_BACK,GL_POINT);
//			glutPostRedisplay();
//			break;

    case 'q':/* Quitter le programme */
        exit(0);
    }
}


void drawPoints(vector<POINT> pointVector)
{
    glBegin(GL_POINTS);
    for(unsigned int i = 0; i < pointVector.size(); i++)
    {
        glVertex2i(pointVector[i].x, pointVector[i].y);
    }
    glEnd();
}

void drawNormals(vector<vec> normals)
{
    glColor3f(1.0,1.0,0.0);
    for(unsigned int i = 0; i < normals.size(); i++)
    {
        glBegin(GL_POINTS);
        glVertex2i(normals[i].x1,normals[i].y1);
        glVertex2i(normals[i].x2, normals[i].y2);
        glEnd();

        glBegin(GL_LINES);
        glVertex2i(normals[i].x1,normals[i].y1);
        glVertex2i(normals[i].x2,normals[i].y2);
        glEnd();
    }
}


bool findPoint(int x, int y, vector<POINT> pointVector, POINT& p)
{
    return false;
}

void VectorNormal(vector<POINT> polygonPoints)
{
    bool clockwise = isClockwise(polygonPoints);
    float X, Y, vectorNormalY, vectorNormalX;

    float norm;

    POINT pt1, pt2;

    windowsNormals.clear();

    for(unsigned int i = 0; i < polygonPoints.size(); i++)
    {
        pt1 = polygonPoints[i];
        pt2 = polygonPoints[(i + 1) % polygonPoints.size()];

        Y = pt1.y + ((pt2.y)-(pt1.y))*0.5;
        X = pt1.x + ((pt2.x)-(pt1.x))*0.5;

        if(clockwise)
        {
            vectorNormalX = ((pt2.y)-(pt1.y));
            vectorNormalY = (-((pt2.x)-(pt1.x)));
        }
        else
        {
            vectorNormalX = (-((pt2.y)-(pt1.y)));
            vectorNormalY = ((pt2.x)-(pt1.x));
        }

        norm = VectorNorm(vectorNormalX, X, vectorNormalY, Y);
        vectorNormalX /= norm/10;
        vectorNormalY /= norm/10;

        vec temp;
        temp.x1 = X;
        temp.y1 = Y;
        temp.x2 = X + vectorNormalX;
        temp.y2 = Y + vectorNormalY;

        windowsNormals.push_back(temp);
    }
}


/** \brief Produit Scalaire
 * x1,x2.. ne sont pas les coordonnées de point, mais les valeurs du vecteur!
 * \param
 * \param
 * \return
 *
 */
float DotProduct(float x1, float x2, float y1, float y2)
{
    return x1*x2 + y1*y2;
}

float DotProduct(vect v1, vect v2)
{
    return v1.x * v2.x + v1.y * v2.y;
}

/** \brief Methode retournant la norme d'un vecteur
 * x1,x2.... coordonnées des points composant le vecteur.
 * \param
 * \param
 * \return
 *
 */
float VectorNorm(float x1, float x2, float y1, float y2)
{
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

//Algo sutherland-Hodgman
void SutherlandHodgman(vector<POINT> polygonPoints, vector<POINT> windowsPoints)
{
    vector<POINT> PS;
    vector<PointWithCnt> polyPointsInside;

    POINT F;
    POINT S;
    POINT I;

    cout << "algo stherland hodgman " << endl;
    polygonPointsWindowed.clear();
    for(unsigned int i = 0; i < windowsPoints.size(); i++)
    {
        PS.clear();
        for(unsigned int j = 0; j < polygonPoints.size(); j++)
        {
            cout << "i : " << i << ", j : " << j << endl;
            if(j == 0)
            {
                F = polygonPoints[j];
            }
            else
            {
                if(cut(windowsNormals[i], S, polygonPoints[j], windowsPoints[i], windowsPoints[(i + 1) % windowsPoints.size()]))
                {
                    cout << "cut" << endl;
                    I = intersect(windowsNormals[i], S, polygonPoints[j], windowsPoints[i], windowsPoints[(i + 1) % windowsPoints.size()]);
                    PS.push_back(I);
                }
            }
            S = polygonPoints[j];
            if(visible(windowsNormals[i], S, windowsPoints[i], windowsPoints[(i + 1) % windowsPoints.size()]))
            {
                //PS.push_back(S);
                addSpecialPoint(polyPointsInside, S);
            }
        }
        if(PS.size() > 0)
        {
            if(cut(windowsNormals[i], S, F, windowsPoints[i], windowsPoints[(i + 1) % windowsPoints.size()]))
            {
                I = intersect(windowsNormals[i], S, F, windowsPoints[i], windowsPoints[(i + 1) % windowsPoints.size()]);
                PS.push_back(I);
            }
            for (int numP = 0; numP < PS.size(); numP++)
                polygonPointsWindowed.push_back(PS[numP]);
        }
        for (int numP = 0; numP < polyPointsInside.size(); numP++)
        {
            if (polyPointsInside[numP].nb == windowsPoints.size())
            {
                polygonPointsWindowed.push_back(polyPointsInside[numP].p);
            }
        }

    }
}

void addSpecialPoint(vector<PointWithCnt> &points, POINT p)
{
    bool found = false;
    for (int i = 0; i < points.size(); i++)
    {
        if (p.x == points[i].p.x && p.y == points[i].p.y)
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
/*
bool cut(POINT currentPolyPoint, POINT otherPolyPoint, POINT windowPoint1, POINT windowPoint2)
{
    cout << "in cut" << endl;

    float a1, a2, b1, b2, commun;

    a1 = (otherPolyPoint.y - currentPolyPoint.y) / (otherPolyPoint.x - currentPolyPoint.x);
    a2 = (windowPoint2.y - windowPoint1.y) / (windowPoint2.x - windowPoint1.x);

    b1 = currentPolyPoint.y - (a1 * currentPolyPoint.x);
    b2 = windowPoint1.y - (a2 * windowPoint1.x);


    if(a1 != a2)
    {
        cout << "a1 ! a2" << endl;
        commun = (b2-b1)/(a1-a2);
        cout << "x1 = " << currentPolyPoint.x << " x2 = " << otherPolyPoint.x << " commun = " << commun << endl;

        if(currentPolyPoint.x < otherPolyPoint.x)
            if((currentPolyPoint.x <= commun && commun <= otherPolyPoint.x))
            {
                cout << "commun1" << endl;
                return true;
            }
        else
            if((otherPolyPoint.x <= commun && commun <= currentPolyPoint.x))
            {
                cout << "commun2" << endl;
                return true;
            }

    }

    return false;
}
*/

bool cut(vec normal, POINT currentPolyPoint, POINT otherPolyPoint, POINT windowPoint1, POINT windowPoint2)
{
    float t;

    t = calcTforIntersect(normal, currentPolyPoint, otherPolyPoint, windowPoint1, windowPoint2);
    if (t >= 0 && t <= 1)
        return true;
    else
        return false;
}

/*
//retournant le point d'intersection [[currentPolyPoint otherPolyPoint] inter (windowPoint1 windowPoint2)
POINT intersect(POINT currentPolyPoint, POINT otherPolyPoint, POINT windowPoint1, POINT windowPoint2)
{
    cout << "intersect" << endl;
    POINT p;
    float a1, a2, b1, b2, X, Y;

    a1 = (otherPolyPoint.y - currentPolyPoint.y) / (otherPolyPoint.x - currentPolyPoint.x);
    a2 = (windowPoint1.y - windowPoint2.y) / (windowPoint1.x - windowPoint2.x);
    b1 = currentPolyPoint.y - (a1 * currentPolyPoint.x);
    b2 = windowPoint1.y - (a2 * windowPoint1.x);
    X = (b2-b1)/(a1-a2);


    Y = a1 * X + b1 ;

    p.x = X;
    p.y = Y;

    return p;
//
//    float mA, mB, mC, mD, iA, iB, iC, iD, multi, xB, yB;
//
//
//
//    mA = otherPolyPoint.x - currentPolyPoint.x;
//    mB = windowPoint1.x - windowPoint2.x;
//    mC = otherPolyPoint.y - currentPolyPoint.y;
//    mD = windowPoint1.y - windowPoint2.y;
//
//    xB = windowPoint1.x - currentPolyPoint.x;
//    yB = windowPoint1.y - currentPolyPoint.y;
//
//    multi =1 /( (mA * mD) - (mB * mC));
//
//    iA = mD * multi;
//    iB = -mB * multi;
//    iC = -mC * multi;
//    iD = mA * multi;
//
//    p.x = iA* xB + iB * yB;
//    p.y = iC * xB + iD * yB;
//
//    return p;

}
*/

float calcTforIntersect(vec normal, POINT A, POINT B, POINT Pi, POINT Pi1)
{
    vect D;
    D.x = B.x - A.x;
    D.y = B.y - A.y;

    vect W;
    W.x = A.x - Pi.x;
    W.y = A.y - Pi.y;

    vect ni;
    ni.x = normal.x2 - normal.x1;
    ni.y = normal.y2 - normal.y1;

    float t = -1;
    float DdotNi = DotProduct(D, ni);
    if (DdotNi != 0)
        t = -(DotProduct(W, ni)) / DotProduct(D, ni);
    return t;
}


POINT intersect(vec normal, POINT A, POINT B, POINT Pi, POINT Pi1)
{
    // [AB] : Q(t) = (1 - t)A + tB, 0 <= t <= 1
    float t;
    float Qt;

    t = calcTforIntersect(normal, A, B, Pi, Pi1);

    POINT I;
    POINT tmpA, tmpB;
    tmpA.x = (1 - t) * A.x;
    tmpA.y = (1 - t) * A.y;

    tmpB.x = t * B.x;
    tmpB.y = t * B.y;

    I.x = tmpA.x + tmpB.x;
    I.y = tmpA.y + tmpB.y;

    return I;
}


//Retourne un booléen si S est visible par rapport à (windowPoint1 windowPoint2

bool visible(vec normal, POINT currentPolyPoint, POINT windowPoint1, POINT windowPoint2)
{
    vect ni;
    ni.x = normal.x2 - normal.x1;
    ni.y = normal.y2 - normal.y1;

    vect FiPi;
    FiPi.x = currentPolyPoint.x - windowPoint1.x;
    FiPi.y = currentPolyPoint.y - windowPoint1.y;


    if(0 <= DotProduct(ni, FiPi))
        return true;
    else
        return false;
}



