//
//  main.cpp
//  cs3388_assignment1_sacar2
//  Copyright © 2016 selin acar. All rights reserved.
//
//  Selin Denise Acar
//  sacar2@uwo.ca
//  250 644 643
//

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#include <cstdlib>
#include <ctime>
#include <vector>

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 640
#define POSITION_X 100
#define POSITION_Y 100
#define WINDOW_NAME "CS3388 - Assignment 1 - Sacar2"

int myWindow;
static GLfloat spin = 0.0;
int randRotationDirections[2] = {-1,1};
bool spinning = 0;


// Generates random floating point numbers between 0 and 1.0
float getRandomFloat(){
    return (float)rand()/RAND_MAX;
}


// A struct defining a rectangles transformations for the rectangles that will be drawn: it's 3D transformations and RGBA colour value
// Depth of object is set to 0 and z scaling is set to 1 because we want flat rects
// Alpha set to 0.6 so we can see through objects if they intersect
// rotation about the z axis is init to either -1 or 1 for (counter)clockwise about the z axis
// An array of 15 rectangles is initialized
struct Rect{
    float translation_x, translation_y, translation_z,
    rotation_theta, rotation_x, rotation_y, rotation_z,
    scaling_x, scaling_y, scaling_z = 1;
    float r = getRandomFloat(), g = getRandomFloat(), b = getRandomFloat(), a = 0.6;
    int rotationDirection = randRotationDirections[rand()%2];
} rectangles[15];


void spinDisplay(void)
{
    spin = spin + 1.0 ;
    if (spin > 360.0)
        spin = spin - 360.0 ;
    glutPostRedisplay() ;
}


// Sets up global attributes, enables transparency, and sets up the desired projection and modelview matrices
// Set camera lens for a perspective viewing volume:
//  the near clipping plane is 0.1 unit from the camera, and the far clipping plane is 80 units away
// Set up ModelView transforms:
//  send the object back 15 units "into the screen", 1 unit down, and 1 units right
//  rotate the transforms to get an angle view 5 degress about the x axis, and 5 degress about the y axis,
//  for a view that looks like you're looking down at the objects from the "top left" corner of the world
void init() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90.0f, 1.0, 0.1f, 80.0f);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(1, -1, -15);
    glRotatef(5, 1, 0, 0);
    glRotatef(5, 0, 1, 0);
    return;
}

// Handle keyboard events
// When 'q' or "Q" is pressed, the window is destroyed and program is terminated
// If 's' or 'S' is pressed, animation begins
void keyboard(unsigned char key, int x, int y){
    
    if (key=='q' || key =='Q'){
        glutDestroyWindow (myWindow);
        exit (0);
    }
    else if (key=='s' || key =='S'){
        if (spinning == false){
            glutIdleFunc(spinDisplay) ;
            spinning = true;
        }else{
            glutIdleFunc(NULL) ;
            spinning = false;
        }
    }
    return;
}


// Display the current state of the system
// Assume the current matrix mode is GL_MODELVIEW
// glClear to clear the window, glFlush to force GL commands, and glutSwapBuffers for animation
void display() {
    
    glClear(GL_COLOR_BUFFER_BIT);
    
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

    // -------------------------------- Draw rectangles -------------------------------
    for (int i = 0; i<15 ; i++){
        
        glPushMatrix(); // Set current matrix on the stack

        glColor4f(rectangles[i].r, rectangles[i].g, rectangles[i].b, rectangles[i].a);
        
        // Rotate either clockwise about z axis, or counter clockwise about z axis
        // Translate to origin, rotate, then Translate to object's POS
        glTranslatef(rectangles[i].translation_x+(rectangles[i].scaling_x/2), rectangles[i].translation_y+(rectangles[i].scaling_y/2), rectangles[i].translation_z+(rectangles[i].scaling_z/2));
        glRotatef(rectangles[i].rotationDirection*spin, 0, 0, 1);
        glTranslatef(-rectangles[i].translation_x-(rectangles[i].scaling_x/2), -rectangles[i].translation_y- (rectangles[i].scaling_y/2), -rectangles[i].translation_z-(rectangles[i].scaling_z/2));
        
        
        glTranslatef(rectangles[i].translation_x, rectangles[i].translation_y, rectangles[i].translation_z);
        glRotatef(rectangles[i].rotation_theta, rectangles[i].rotation_x, rectangles[i].rotation_y, rectangles[i].rotation_z);
        glScalef(rectangles[i].scaling_x, rectangles[i].scaling_y, rectangles[i].scaling_z);

        glBegin(GL_QUADS);
                glVertex3i(0,1,0);
                glVertex3i(0,0,0);
                glVertex3i(1,0,0);
                glVertex3i(1,1,0);
        glEnd();
        
        glPopMatrix(); // Pop the old matrix without the transformations.
    }
    
    glFlush();
    glutSwapBuffers();
    return;
}


// Create rectangles: generate random colour, initial position, orientation, and dimension
// Seeds randomizer for new value each time program is run
void initRects(){
    srand(time(NULL));
    
    for (int i = 0; i<15 ; i++){
        rectangles[i].r = getRandomFloat(),
        rectangles[i].g = getRandomFloat(),
        rectangles[i].b = getRandomFloat(),
        
        rectangles[i].translation_x = (float)(rand()%17-8),
        rectangles[i].translation_y = (float)(rand()%17-8),
        rectangles[i].translation_z = (float)(-rand()%8),
        
        rectangles[i].rotation_theta = rand()%360,
        rectangles[i].rotation_x = rand()%2,
        rectangles[i].rotation_y = rand()%2,
        rectangles[i].rotation_z = rand()%2,
        
        rectangles[i].scaling_x = rand()%5+1,
        rectangles[i].scaling_y = rand()%5+1;
    }
}


// Initializes GLUT, the display mode, main window, applicaion, and enters the main event loop.
// - init the GLUT library (utility toolkit to open a window, get input, etc.)
// - set display modes with a double buffer &RGBA mode rather than colour_index mode
//      RGBA mode: hardware sets aside a certain number of bitplanes for each of the R, G, B, and A components
//      color_index mode: OpenGL uses a color map (or lookup table), which is similar to using a palette to mix paints
// Window is initialized at 100x100, with a size 640px × 640px, and a name of "CS3388 - Assignment 1"
// Display callback is called, as well as a keyboard callback so that keyboard events can be handled
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowPosition(POSITION_X, POSITION_Y);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    myWindow = glutCreateWindow(WINDOW_NAME);
    
    initRects();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    init();
    glutMainLoop();
    
    return 0;
}
