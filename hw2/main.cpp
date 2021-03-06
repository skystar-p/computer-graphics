#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/freeglut.h>
#include "display.h"
#include "camera.h"
#include <cstdio>

extern GLdouble rot, rot_sphere1, rot_sphere2;
extern GLdouble rot_card;
extern GLUquadricObj *obj;

Camera camera(Vec(0.0, 0.0, 100.0));

int left_mouse_button = 0;
int right_mouse_button = 0;
int last_x = 0, last_y = 0;

int width = 0, height = 0;
unsigned timestep = 10;

int zoom_mode = 0;

void init() {
    rot = 0.0;
    rot_sphere1 = 0.0;
    rot_sphere2 = 0.0;
    rot_card = 0.0;
    obj = gluNewQuadric();
    gluQuadricDrawStyle(obj, GLU_FILL);
    gluQuadricNormals(obj, GLU_SMOOTH);
    gluQuadricOrientation(obj, GLU_OUTSIDE);
    gluQuadricTexture(obj, GL_FALSE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glShadeModel(GL_SMOOTH);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

void resize(int w, int h) {
    width = w;
    height = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (GLfloat)w / (GLfloat)h, .1f, 500.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 27:
        exit(0);
        break;
    case 'z':
        if (zoom_mode == 0) {
            zoom_mode = 1;
            printf("Zoom mode\n");
        }
        break;
    case 'x':
        if (zoom_mode == 1) {
            zoom_mode = 0;
            printf("Dolly mode\n");
        }
        break;
    case 's':
        camera.show_all();
        break;
    default:
        break;
    }
}

void timer(int unused) {
    glutPostRedisplay();

    // rotation degree manipulation
    if (rot > 360.0) rot -= 360.0;
    rot += 2.5;

    if (rot_sphere1 > 360.0) rot_sphere1 -= 360.0;
    rot_sphere1 += 2.0;

    if (rot_sphere2 > 360.0) rot_sphere2 -= 360.0;
    rot_sphere2 += 1.5;

    if (rot_card > 360.0) rot_card -= 360.0;
    rot_card += 1.0;


    glutTimerFunc(timestep, timer, 0);
}

void mouse_click(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        left_mouse_button = 1;
        camera.update_vectors();
        last_x = x; last_y = y;
    }
    else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
        left_mouse_button = 0;
    }

    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        right_mouse_button = 1;
        camera.update_vectors();
        last_x = x; last_y = y;
    }
    else if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP) {
        right_mouse_button = 0;
    }

    if (zoom_mode == 0) {
        if (button == 3 && state == GLUT_DOWN) {
            camera.dolly(-10.0f);
        }

        if (button == 4 && state == GLUT_DOWN) {
            camera.dolly(10.0f);
        }
    }
    else {
        if (button == 3 && state == GLUT_DOWN) {
            camera.zoom(-3.0f);
        }

        if (button == 4 && state == GLUT_DOWN) {
            camera.zoom(3.0f);
        }
    }
}

void mouse_move(int x, int y) {
    if (left_mouse_button) {
        Vec first = camera.to_sphere(last_x, last_y, width, height);
        Vec last = camera.to_sphere(x, y, width, height);

        camera.rotate(first, last);
    }
    else if (right_mouse_button) {
        camera.translate(Vec(last_x - x, last_y - y, 0.0f));
    }
}

void register_callbacks() {
    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(timestep, timer, 0);
    glutMouseFunc(mouse_click);
    glutMotionFunc(mouse_move);
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 800);
    glutInitWindowPosition(50, 0);
    glutCreateWindow("Homework 2");

    register_callbacks();
    init();

    glutMainLoop();
    return 0;
}
