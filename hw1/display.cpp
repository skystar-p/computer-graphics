#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/freeglut.h>
#include <iostream>

float eye[3] = { -0.0, -0.0, 80.0 };
float ori[3] = { 0.0, 0.0, 0.0 };

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(eye[0], eye[1], eye[2], ori[0], ori[1], ori[2], 0, 1, 0);

    glTranslatef(0.0, 0.0, -50.0);

    glBegin(GL_POLYGON);
        glColor3f(0.0, 1.0, 0.0);
        glVertex3f(10.0, 10.0, -10.0);
        glVertex3f(-10.0, 10.0, -10.0);
        glVertex3f(-10.0, -10.0, -10.0);
        glVertex3f(10.0, -10.0, -10.0);
    glEnd();

    glutSwapBuffers();
}
