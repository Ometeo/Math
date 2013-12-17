#include <windows.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <iostream>

using namespace std;

int x0, y0;
double c;
bool drawPoly;


vector<POINT> windowsPoints;
vector<POINT> polygonPoints;


void init(int argc, char **argv);
void display();
void keyboard(unsigned char button, int x, int y);
void mouse(int mouseButton, int state, int x, int y);

void drawPoints(vector<POINT> pointVector);
bool findPoint(int x, int y, vector<POINT> pointVector, POINT& p);




int main (int argc, char **argv)
{
    drawPoly = true;
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
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(1.0, 0.0, 0.0);
    drawPoints(windowsPoints);

    if(windowsPoints.size() > 1)
    {
        for(int i = 1; i < windowsPoints.size(); i++)
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
        for(int i = 1; i < polygonPoints.size(); i++)
        {
                glBegin(GL_LINES);
                    glVertex2i(polygonPoints[i-1].x, polygonPoints[i-1].y);
                    glVertex2i(polygonPoints[i].x, polygonPoints[i].y);
                glEnd();
        }
        glBegin(GL_LINES);
            glVertex2i(polygonPoints[polygonPoints.size()-1].x, polygonPoints[polygonPoints.size()-1].y);
            glVertex2i(polygonPoints[0].x, polygonPoints[0].y);
        glEnd();
    }

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
            windowsPoints.push_back(temp);
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
                cout << "null" << endl;
            }
        }
    }
}

void keyboard(unsigned char touche,int x,int y){
	switch (touche){
        case 'w':
            drawPoly = false;
            break;
        case 'p':
            drawPoly = true;
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
    for(int i = 0; i < pointVector.size(); i++)
    {
        glVertex2i(pointVector[i].x, pointVector[i].y);
    }
    glEnd();
}

bool findPoint(int x, int y, vector<POINT> pointVector, POINT& p)
{
    return false;
}
