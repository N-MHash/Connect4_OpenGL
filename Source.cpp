// mohamed.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"
using namespace std;
#include <stdlib.h>
#include<iostream>
#include <glut.h>
#include <vector>
#include <math.h>

#define pi 3.14159265358979

struct cell_center_param {
	float position_x;
	float position_y;
};

vector<cell_center_param> cell_center(42);

float circle_center_x = 0, circle_center_y = 0;
float circle_radius;
//<<<<<<<<<<<<<<<<<<<<<<< myInit >>>>>>>>>>>>>>>>>>>>
void myInit(void)
{
	glClearColor(1.0, 1.0, 1.0, 0.0); // set white background color
	glColor3f(0.0f, 0.0f, 0.0f); // set the drawing color
	glPointSize(4.0); // a ‘dot’ is 4 by 4 pixels
	gluOrtho2D(0.0, 640.0, 0.0, 480.0);
}

//<<<<<<<<<<<<<<<<<<<<<<<< myDisplay >>>>>>>>>>>>>>>>>
void myDisplay(void)
{
	glClear(GL_COLOR_BUFFER_BIT); // clear the screen

	glBegin(GL_QUADS);
		glColor3f(0.168f, 0.294f, 0.9f);
		glVertex2i(20, 63);
		glVertex2i(540, 63);
		glVertex2i(540, 410);
		glVertex2i(20, 410);
	glEnd();

	int current_cell = -1;
	for (int i = 1; i <= 7;i++)
	{
		circle_center_y = 45.7;
		for (int k = 1; k <= 6;k++)
		{
			current_cell += 1;
			
			glBegin(GL_TRIANGLE_FAN);
				glColor3f(1.0f, 1.0f, 1.0f);
			
				circle_center_x = 70*i;
				circle_center_y += 55.7;
				circle_radius = 25;
			
				glVertex2f(circle_center_x, circle_center_y);
			
				cell_center[current_cell].position_x = circle_center_x;
				cell_center[current_cell].position_y = circle_center_y;
			
				for (int j = 0; j <= 20; j++)
				{
					glVertex2f(circle_center_x + (cos(j * 2 * pi / 20)*circle_radius), circle_center_y + (sin(j * 2 * pi / 20) *circle_radius));
				}
			glEnd();	
		}
	}
	glFlush(); 
}
//<<<<<<<<<<<<<<<<<<<<<<<< main >>>>>>>>>>>>>>>>>>>>>>
void main(int argc, char** argv)
{
	glutInit(&argc, argv); // initialize the toolkit
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); // set display mode
	glutInitWindowSize(640, 480); // set window size
	glutInitWindowPosition(300, 150); // set window position on screen
	glutCreateWindow("points"); // open the screen window
	glutDisplayFunc(myDisplay); // register redraw function
	myInit();
	glutMainLoop(); // go into a perpetual loop
}
