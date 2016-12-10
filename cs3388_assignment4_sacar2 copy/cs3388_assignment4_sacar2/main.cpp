/* This program demonstrates how to use light sources, front and back
 faces, and Z-buffer techniques to render objects with realism */

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <math.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define WIN_X        800.0
#define WIN_Y        640.0
#define F           2000.0
#define N            100.0

#define MSEC            25

#define RADIUS       150.0
#define HEIGHT       200.0

#define INNER_RADIUS  30.0
#define OUTER_RADIUS  70.0

#define AXIS_LENGTH 1000.0

#define PARAM_RADIUS 800.0
#define PARAM_AMPLIT 250.0

#define GAZEx          0.0
#define GAZEy          0.0
#define GAZEz          0.0

#define UPx            0.0
#define UPy            0.0
#define UPz            1.0

#define VIEW_ANGLE    80.0
#define ASPECT_RATIO   1.0

#define M_PI 3.14

GLfloat ambientlight[] = {0.3,0.3,0.3,1.0} ;
GLfloat diffuselight[] = {0.7,0.7,0.7,1.0} ;
GLfloat specular[] = {1.0,1.0,1.0,1.0} ;
GLfloat specref[] = {1.0,1.0,1.0,1.0} ;
GLfloat lightposition[] = {0.0,0.0,0.0} ;


GLfloat t = 0.0 ;
GLfloat theta = 0.0 ;

void RenderScene(void) {
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) ;
    
    glLineWidth(3.0) ;
    glBegin(GL_LINES) ;
    glColor3f(0.0,1.0,1.0) ;
    glVertex3f(0.0,0.0,0.0) ;
    glVertex3f(0.0,0.0,AXIS_LENGTH) ;                      /* Draw Z-axis */
    
    glColor3f(0.0,1.0,0.0) ;
    glVertex3f(0.0,0.0,0.0) ;
    glVertex3f(0.0,AXIS_LENGTH,0.0) ;                      /* Draw Y-axis */
    
    glColor3f(1.0,0.0,0.0) ;
    glVertex3f(0.0,0.0,0.0) ;
    glVertex3f(AXIS_LENGTH,0.0,0.0) ;                      /* Draw X-axis */
    glEnd() ;
    glLineWidth(1.0) ;
    
    glPushMatrix() ;
    glTranslatef(300.0,300.0, 0.0) ;
    glColor3f(0.37,0.35,0.4) ;
    /* Torus @ (300,300,1) */
    glutSolidSphere(INNER_RADIUS, 240, 240);
//    glutSolidTorus(INNER_RADIUS,OUTER_RADIUS,240,240) ;
    glPopMatrix() ;
    
    glPushMatrix() ;
    glTranslatef(0.0,-300.0, 0.0) ;
    glColor3f(0.37,0.35,0.4) ;
    /* Torus @ (300,300,1) */
    glutSolidSphere(OUTER_RADIUS, 240, 240);
    //    glutSolidTorus(INNER_RADIUS,OUTER_RADIUS,240,240) ;
    glPopMatrix() ;
    
    glutSwapBuffers() ;
}

void TimerFunction(int value) {
    
    glMatrixMode(GL_PROJECTION) ;     /* Operation on the Projection matrix */
    glLoadIdentity() ;
    gluPerspective(VIEW_ANGLE,ASPECT_RATIO,N,F) ;     /* Define view volume */
    glMatrixMode(GL_MODELVIEW) ;       /* Operation on the modelview matrix */
    glLoadIdentity() ;
    /* light source at origin  of the viewing volume so that it moves with */
    /* the camera */
    glLightfv(GL_LIGHT0,GL_POSITION,lightposition) ;
    /* Position the camera */
    gluLookAt(PARAM_RADIUS*cos(t),PARAM_RADIUS*sin(t),2500,0,0,0,0.0,0.0,1) ;
//    extern void gluLookAt (GLdouble eyeX, GLdouble eyeY, 2500, GLdouble centerX, GLdouble centerY, GLdouble centerZ, GLdouble upX, GLdouble upY, GLdouble upZ) OPENGL_DEPRECATED_MSG(10_0, 10_9, "Use GLKMatrix4MakeLookAt");

    if (t >= M_PI*2.0) t = 0.0 ;
    t += 0.01 ;            /* Progress in the path of the parametric circle */
    if (theta >= M_PI*2.0) theta = 0.0 ;
    /* Go under and over the scene using a sine wave for height */
    theta += 0.0125 ;
    glutPostRedisplay() ;
    glutTimerFunc(MSEC,TimerFunction,1) ;
    
//    /* Position the camera */
//    gluLookAt(PARAM_RADIUS*cos(t),PARAM_RADIUS*sin(t),PARAM_AMPLIT*sin(theta),GAZEx,GAZEy,GAZEz,UPx,UPy,UPz) ;
//    if (t >= M_PI*2.0) t = 0.0 ;
//    t += 0.01 ;            /* Progress in the path of the parametric circle */
//    if (theta >= M_PI*2.0) theta = 0.0 ;
//    /* Go under and over the scene using a sine wave for height */
//    theta += 0.0125 ;
//    glutPostRedisplay() ;
//    glutTimerFunc(MSEC,TimerFunction,1) ;
}

void ChangeSize(int w, int h) {
    
    glViewport(0,0, (GLsizei) w, (GLsizei) h) ;
    glMatrixMode(GL_PROJECTION) ;
    glLoadIdentity() ;
//    glOrtho((double)-WIN_Y/2,(double)WIN_X/2,(double)-WIN_Y/2,(double)WIN_Y/2,N,F);
    glOrtho(0,500,500,0,-1, 1);
    glMatrixMode(GL_MODELVIEW) ;
    
    //Set your viewing window l, r, b, t to match your glOrtho (e.g. glOrtho(0, 500, 500, 0, -1, 1). When you set your camera, put the eye far down the Z axis (e.g. 2500), pointing towards the origin. Then you can place your spheres in the window (e.g. sphere x=250, y=250, z=0, radius=100)
    
    glLoadIdentity() ;
}

int main(int argc, char** argv) {
    
    glutInit(&argc,argv) ;
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB) ;  /* Z-buffer */
    glutInitWindowSize((int)WIN_X,(int)WIN_Y) ;
    glutInitWindowPosition(0,0) ;
    glutCreateWindow(argv[0]) ;
    glutDisplayFunc(RenderScene) ;
    glutReshapeFunc(ChangeSize) ;
    
    glClearColor(0.3,0.4,0.5,1.0) ;
    
    glEnable(GL_DEPTH_TEST) ;                           /* Enable Z-buffer */
    glFrontFace(GL_CCW) ;            /* Counterclockwise polygons face out */
    /* Force true normals */
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,GL_TRUE) ;
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,GL_TRUE) ;  /* Front @ back faces */
    glShadeModel(GL_SMOOTH) ;                            /* Gouraud shading */
    glEnable(GL_LIGHTING) ;
    
    glLightfv(GL_LIGHT0,GL_AMBIENT,ambientlight) ;
    glLightfv(GL_LIGHT0,GL_DIFFUSE,diffuselight) ;
    glLightfv(GL_LIGHT0,GL_SPECULAR,specular) ;
    
    glEnable(GL_LIGHT0) ;
    glEnable(GL_COLOR_MATERIAL) ;
    glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE) ;
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,specref) ;
    glMateriali(GL_FRONT_AND_BACK,GL_SHININESS,16) ;
    
    glutTimerFunc(MSEC,TimerFunction,1) ;
    glutMainLoop();
    return 0;
}
