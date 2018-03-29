#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/freeglut.h>
#include <iostream>
#include <cmath>

GLdouble rot, rot_sphere1, rot_sphere2;
GLdouble rot_card;
GLUquadricObj *obj;

int card_count = 10;

float eye[3] = { 0.0, 0.0, 30.0 };
float ori[3] = { 0.0, 0.0, 0.0 };

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(eye[0], eye[1], eye[2], ori[0], ori[1], ori[2], 0, 1, 0);

    glTranslatef(0.0, 0.0, -60.0);

    glPushMatrix(); {
        glColor3f(1.0, 1.0, 1.0);
        glRotatef(90.0, -sqrt(2), sqrt(2), 0.0);
        glTranslatef(0.0, 0.0, -38.0);
        gluCylinder(obj, 0.3, 0.3, 70.0, 64, 16);
        gluCylinder(obj, 4.0, 4.0, 1.0, 64, 16);
        glTranslatef(0.0, 0.0, 70.0);
        gluCylinder(obj, 3.0, 0, 10.0, 64, 16);
    } glPopMatrix();

    // rotating cards
    glPushMatrix(); {
        glColor4f(0.0, 1.0, 1.0, 1.0);
        glRotatef(rot_card, 0.0, 1.0, 0.0);
        glRotatef(15.0, 0.0, 0.0, -1.0);
        for (int i = 0; i < card_count; i++) {
            glRotatef(360.0 / card_count, 0.0, 1.0, 0.0); glBegin(GL_QUADS);
                glVertex3f(3.0, 5.0, -30);
                glVertex3f(-3.0, 5.0, -30);
                glVertex3f(-3.0, -5.0, -30);
                glVertex3f(3.0, -5.0, -30);
            glEnd();
        }
    } glPopMatrix();

    glPushMatrix(); { // outermost ring
        glColor3f(1.0, 1.0, 1.0);
        glutSolidTorus(0.5, 20.0, 64, 64);

        glPushMatrix(); { // first ring
            glRotatef(rot, 1.0, 0.0, 0.0);
            glColor3f(1.0, 1.0, 1.0);
            glutSolidTorus(0.5, 17.0, 64, 64);

            glPushMatrix(); { // sphere on first ring
                glRotatef(rot_sphere1, 0.0, 0.0, 1.0);
                glTranslatef(0.0, 17.0, 0.0);
                glColor3f(1.0, 0.0, 0.0);
                gluSphere(obj, 3.0, 64, 16);
            } glPopMatrix();


            glPushMatrix(); { // second ring
                glRotatef(90.0, sqrt(2), sqrt(2), 0.0);
                glColor3f(1.0, 1.0, 1.0);
                glutSolidTorus(0.5, 14.0, 64, 64);

                // innermost cylinder
                glPushMatrix(); {
                    glRotatef(90.0, 0.0, 1.0, 0.0);
                    gluCylinder(obj, 10.0, 10.0, 1.0, 64, 16);
                } glPopMatrix();

                glPushMatrix(); { // sphere on second ring
                    glRotatef(rot_sphere2, 0.0, 0.0, 1.0);
                    glTranslatef(0.0, 14.0, 0.0);
                    glColor3f(0.0, 1.0, 0.0);
                    gluSphere(obj, 3.0, 64, 16);
                } glPopMatrix();

            } glPopMatrix();

        } glPopMatrix();

    } glPopMatrix();

    glutSwapBuffers();
}
