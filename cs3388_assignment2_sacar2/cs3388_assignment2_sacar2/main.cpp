//
//  main.cpp
//  cs3388_assignment2_sacar2
//  Copyright © 2016 selin acar. All rights reserved.
//
//  Created by Selin Denise Acar on 2016-10-25.
//  sacar2@uwo.ca
//  250 644 643
//
/*  Implementation of a wiremesh renderer.
    Creating a 3D vase defined around an axis of symmetry */

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <math.h>

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 640
#define POSITION_X 100
#define POSITION_Y 100
#define WINDOW_NAME "CS3388 - Assignment 2 - Sacar2"
#define MAX_CHARS_PER_LINE 100
#define MAX_TOKENS_PER_LINE 4
#define DELIMITER " "
#define PI 3.14159265389

//rotation angles, start with -90 in x to get an upright vase
int anglex= -90, angley = 0, anglez = 0;
int myWindow;
int NumOfProfiles;

//Data Structure to store Points
struct point{
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;
    double w = 0.0;
};
std::vector<point> points;

/* Set up global attributes, enables transparency, set up the projection/modelview matrices, and polygon mode
 Set camera lens for a perspective viewing volume:
 the near clipping plane is 0.1 unit from the camera, and the far clipping plane is 80 units away
 ModelView transforms setup: send the object back 40 units "into the screen", 20 units down
 rotate the transforms to get an angle view 15 degress about the x axis, and 15 degress about the y axis,
 for a view that looks like you're looking down at the objects from the "top left" corner of the world
 Polygon mode sets the vase to appear as a mesh rather than solid
 */
void init() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90.0f, 1.0, 0.1f, 80.0f);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(1, -20, -40);
    glRotatef(-15, 1, 0, 0);
    glRotatef(-15, 0, 1, 0);
    
    glPolygonMode(GL_FRONT, GL_LINE) ;
    glPolygonMode(GL_BACK,  GL_LINE) ;
    glShadeModel(GL_FLAT);
    
    return;
}

/* Read the profile points from vase.txt
 Parse the 3D object data into a point object, and put point into a vector which holds all these points from the profile txt file
 */
void parseProfile(std::string fileName)
{
    std::ifstream inStream;     // create a file reading object
    inStream.open(fileName);    // open the file
    
    if (!inStream.good()){
        std::cout << "Input file not found: "<<fileName<<"\n";
        exit(1);
    }
    
    //while we haven't reached the end of the file, read each line
    while ( !inStream.eof()){
        char buf[MAX_CHARS_PER_LINE];                   // read a line into memory
        inStream.getline(buf, MAX_CHARS_PER_LINE);
        const char* vertex[MAX_TOKENS_PER_LINE] = {};   // array to store memory addresses of the tokens in buffer. init to 0
        vertex[0] = strtok(buf, DELIMITER);             // parse the line by the blank space character as the delimiter
        
        if (vertex[0])
        {
            for (int index = 1; index < MAX_TOKENS_PER_LINE; index++)
            {
                vertex[index] = strtok(0, DELIMITER);
                if (!vertex[index]) break;              // break if no more tokens
            }
            point thisPoint = *new point;
            thisPoint.x = atof(vertex[0]);
            thisPoint.y = atof(vertex[1]);
            thisPoint.z = atof(vertex[2]);
            thisPoint.w = atof(vertex[3]);
            points.push_back(thisPoint);
        }
    }
    
    inStream.close();
    std::cout << "Read file completed"<< std::endl;
}

//helps find next x & y points for the vase
float findPt(std::vector<point>  points, float z, int n_points )
{
    float num = 1.0, den = 1.0, sum = 0.0;
    for (int i = 0; i < n_points; i++){
        num = den = 1.0;
        for (int j = 0; j < n_points; j++) {
            if (j != i) num = num * (z - points[j].z);
        }
        for (int j = 0; j < n_points; j++) {
            if (j != i) den = den * (points[i].z - points[j].z);
        }
        sum += num / den * points[i].x;
    }
    return sum;
}


// Display the current state of the system
// Assume the current matrix mode is GL_MODELVIEW
// glClear to clear the window, glFlush to force GL commands, and glutSwapBuffers for animation
// Draw vase Points, mesh, and the coordinate system lines to help navigate the view
void display(){
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.6, 0.7, 1.0);
    
    glPushMatrix();                         //rotations
    glRotatef(anglex, 1.0, 0.0, 0.0);
    glRotatef(angley, 0.0, 1.0, 0.0);
    glRotatef(anglez, 0.0, 0.0, 1.0);
    
    const float startz = 0, endz = 26;
    const int   slices_z = 20;                      // # slices along z
    const int   slices_theta = 360/2;               // # angular slices
    const float dz = (endz - startz)/slices_z;      //z step size
    const float dtheta = 2*PI / slices_theta;		//angular step size
    
    int i, j;
    float x, y, z = points[0].z, r = findPt(points, z, 26), x1, y1, z1, r1, theta = 0;
    
    for (i = 0; i<slices_z; i++) {
        z1 = z + dz;                        //next z
        r1 = findPt(points, z1, 21);         //next f(z)
        
        //sweep theta to draw vase
        //forms quad with next pair of points with incremented theta value
        glBegin( GL_QUAD_STRIP );
        for (j = 0; j<=slices_theta; j++) {
            theta += dtheta;
            double cosa = cos(theta);
            double sina = sin (theta);
            x = r * cosa;   x1 = r1 * cosa;
            y = r * sina;	y1 = r1 * sina;
            
            //edge from point at x to point at next x
            glVertex3f(x, y, z);
            glVertex3f(x1, y1, z1);
        }
        glEnd();
        z = z1;
        r = r1;
    }
    
    glPointSize(2.0);
    double w = points[i].w;
    glColor3f(0.7, 0.8, 1.0);
    
    glBegin(GL_POINTS);
    for (i = 0; i < points.size(); i++){
        glVertex3i(points[i].x/w,points[i].y/w,points[i].z/w);
    }
    glEnd();
    glPopMatrix();
    glFlush();

    // ------------------- setup an axis to help see what's going on ------------------
    glColor3f(1, 1, 1);
    
    glBegin(GL_LINES);
    glVertex3i(0,-14,0);
    glVertex3i(0,14,0);
    glEnd();
    
    glBegin(GL_LINES);
    glVertex3i(-14,0,0);
    glVertex3i(14,0,0);
    glEnd();
    
    glBegin(GL_LINES);
    glVertex3i(0,0,0);
    glVertex3i(0,0,-40);
    glEnd();

    glFlush();
    glutSwapBuffers();  //THIS IS IMPORTANT!!! MAKES EVERYTHING APPEAR SINCE YOU'RE USING DOUBLE BUFFERS
    
    return;
}


//Using to rotate model upon varied axes by 2 degrees per key press
void keyboard(unsigned char key, int x, int y)
{
    switch(key) {
        case 'x':
            anglex = (anglex + 2) % 360;
            break;
        case 'X':
            anglex = (anglex - 2) % 360;
            break;
        case 'y':
            angley = (angley + 2) % 360;
            break;
        case 'Y':
            angley = (angley - 2) % 360;
            break;
        case 'z':
            anglez = (anglez + 2) % 360;
            break;
        case 'Z':
            anglez = (anglez - 2) % 360;
            break;
        case 'q': case'Q':
            glutDestroyWindow(myWindow);
            exit(0);
    }
    glutPostRedisplay();
}


int main(int argc, char** argv){
    parseProfile("vase.txt");
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowPosition(POSITION_X, POSITION_Y);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    myWindow = glutCreateWindow(WINDOW_NAME);
    
    glutDisplayFunc(display);
    init();
    glutKeyboardFunc (keyboard);
    glutMainLoop();

    return 0;
}
