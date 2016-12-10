//
//  main.cpp
//  cs3388_assignment4_sacar2
//
//  Created by selin acar on 2016-12-04.
//  Copyright © 2016 selin acar. All rights reserved.
//

#include <iostream>
#include <vector>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#include <math.h>
#include "Camera.hpp"
#include "Vec3.hpp"
#include "SceneObject.hpp"
#include "Sphere.hpp"
#include "Ground.hpp"

#include "Constants.h"
#define WINDOW_NAME "CS3388 - Assignment 4 - Sacar2"
int  myWindow;

std::vector<std::shared_ptr<SceneObject> > scene;
Vec3 light(120,400,300);
Camera camera(Vec3(-1, -1, -1),Vec3(0,1,0));

/* init()
 Set camera lens for a orthogonic viewing volume
 Set viewing window / glOrtho (e.g. glOrtho(0, 640, 500, 0, -1, 1).
 Place spheres in the window
 create ground
*/
void init() {
    // ------------ set the shade model ----------------
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, Constants::WINDOW_WIDTH, Constants::WINDOW_HEIGHT, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

//    scene.push_back(std::shared_ptr<SceneObject>(new Ground(0.9, 1, 0.5, Vec3(-20,-20,0))));
    scene.push_back(std::shared_ptr<SceneObject>(new Sphere(1.0,0.0,0.0,Vec3(15,15,0), 8)));
    scene.push_back(std::shared_ptr<SceneObject>(new Sphere(0.0,1.0,0.0,Vec3(-15,5,10), 7)));
    scene.push_back(std::shared_ptr<SceneObject>(new Sphere(0.0,0.0,1.0,Vec3(0,5,5), 9)));

    return;
}

/* keyboard()
 Handle keyboard events
 When 'q' or "Q" is pressed, the window is destroyed and program is terminated
*/
void keyboard(unsigned char key, int x, int y){
    
    if (key=='q' || key =='Q'){
        glutDestroyWindow (myWindow);
        exit (0);
    }
    return;
}


/* Display the current state of the system
 glClear to clear the window, glFlush to force GL commands, and glutSwapBuffers for animation
*/
void display() {
    
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_POINTS);
    camera.draw(scene, light);
    glEnd();
    glFlush();
    glutSwapBuffers();
    return;
}

/* Add animation: bounces the balls*/
void bounceBalls(void)
{
    glutPostRedisplay() ;
}



/* Initializes GLUT, the display mode, main window, application, and enters the main event loop.
 - init the GLUT library (utility toolkit to open a window, get input, etc.)
 - set display modes with a double buffer &RGBA mode rather than colour_index mode
      RGBA mode: hardware sets aside a certain number of bitplanes for each of the R, G, B, and A components
      color_index mode: OpenGL uses a color map (or lookup table), which is similar to using a palette to mix paints
 Window is initialized at 100x100, with a size 640px × 500px, and a name of "CS3388 - Assignment 1"
 Display callback is called, as well as a keyboard callback so that keyboard events can be handled
 */
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowPosition(Constants::POSITION_X, Constants::POSITION_Y);
    glutInitWindowSize(Constants::WINDOW_WIDTH, Constants::WINDOW_HEIGHT);
    myWindow = glutCreateWindow(WINDOW_NAME);
    
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    init();
    glutMainLoop();
    
    return 0;
}
