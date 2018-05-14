#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/freeglut.h>
#include <iostream>
#include <string>
#include <algorithm>
#include "camera.h"
#include "parse.h"
#include "polygon.h"


int SPLINE_DIVISION = 20;

int left_mouse_button = 0;
int right_mouse_button = 0;
int last_x = 0, last_y = 0;
int zoom_mode = 0;

int width = 0, height = 0;
unsigned timestep = 10;

Camera camera(Vec(0.0, 0.0, 100.0));
GLfloat camera_pos[3] = { 0.0f, 0.0f, 100.0f };
Surface surface;

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
        camera.lookat();
    }
    else if (right_mouse_button) {
        camera.translate(Vec(last_x - x, last_y - y, 0.0f));
        camera.lookat();
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClearColor(0.0f, 1.0f, 0.0f, 0.0f);

    camera.lookat();

    std::vector<std::vector<Polygon>> polygons;

    const unsigned ii = surface.sections.size() - 1;
    for (unsigned i = 0; i < ii - 1; i++) {
        const unsigned c = surface.sections[i].points.size();
        std::vector<Polygon> inner_polys;
        for (unsigned j = 0; j < c; j++) {
            glm::vec3 v0 = surface.sections[i].points[j];
            glm::vec3 v1 = surface.sections[i + 1].points[j];
            glm::vec3 v2 = (j == c - 1) ?
                surface.sections[i + 1].points[0] : surface.sections[i + 1].points[j + 1];
            glm::vec3 v3 = (j == c - 1) ?
                surface.sections[i].points[0] : surface.sections[i].points[j + 1];

            glm::vec3 normal = glm::normalize(glm::cross(v0 - v2, v1 - v3));
            glm::vec3 camera_vec = glm::vec3(camera_pos[0], camera_pos[1], camera_pos[2]);
            
            if (glm::dot(camera_vec, normal) > 0.0f) {
                inner_polys.push_back(Polygon(v0, v1, v2, v3, normal, true));
            } else {
                inner_polys.push_back(Polygon(v0, v1, v2, v3, normal, false));
            }
        }
        polygons.push_back(inner_polys);
    }

    for (unsigned i = 0; i < ii - 1; i++) {
        const unsigned c = surface.sections[i].points.size();
        for (unsigned j = 0; j < c; j++) {
            unsigned im = (i - 1) + (i == 0);
            unsigned jm = (j + c - 1) % c;
            glm::vec3 norm_up = polygons[i][j].normal;
            glm::vec3 norm_down = polygons[im][jm].normal;
            glm::vec3 norm_left = polygons[i][jm].normal;
            glm::vec3 norm_right = polygons[im][j].normal;

            polygons[i][j].new_normal = glm::normalize(norm_up + norm_down + norm_left + norm_right);
        }
    }

    for (unsigned i = 0; i < ii - 1; i++) {
        const unsigned c = surface.sections[i].points.size();
        for (unsigned j = 0; j < c; j++) {
            unsigned ip = (i + 1) - (i == ii - 2);
            unsigned jp = (j + c + 1) % c;
            polygons[i][j].normal = polygons[i][j].new_normal;
            polygons[i][j].r_normal = polygons[ip][j].new_normal;
            polygons[i][j].u_normal = polygons[i][jp].new_normal;
            polygons[i][j].ru_normal = polygons[ip][jp].new_normal;
        }
    }

    for (auto& poly_vec : polygons) {
        for (auto& poly : poly_vec) {
            if (!poly.is_front) {
                poly.draw();
            }
        }
    }

    for (auto& poly_vec : polygons) {
        for (auto& poly : poly_vec) {
            if (poly.is_front) {
                poly.draw();
            }
        }
    }
    // std::sort(polygons.begin(), polygons.end());

    glutSwapBuffers();
}

void timer(int unused) {
    glutPostRedisplay();
    glutTimerFunc(timestep, timer, 0);
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
    glutCreateWindow("Homework 3");

    // enable lighting
    glEnable(GL_LIGHTING);

    GLfloat light0_pos[] = { 0.0f, 50.0f, 200.0f, 0.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, light0_pos);
    glEnable(GL_LIGHT0);

    // enable depth test
    glEnable(GL_DEPTH_TEST);

    glShadeModel(GL_SMOOTH);

    register_callbacks();

    std::string path;

    if (argc < 2) {
        path = "chess.in";
    }
    else {
        path = argv[1];
    }

    surface = read_from_file(path);

    glutMainLoop();
    return 0;
}
