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

bool light_toggle[4] = { true, false, false, false };

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

    case '1':
    case '2':
    case '3':
    case '4': {
        int ind = key - '1';
        light_toggle[ind] = !light_toggle[ind];
        if (light_toggle[ind]) {
            glEnable(GL_LIGHT0 + ind);
            printf("Light %d on\n", ind + 1);
        } else {
            glDisable(GL_LIGHT0 + ind);
            printf("Light %d off\n", ind + 1);
        }
        break;
    }
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
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

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

            inner_polys.push_back(Polygon(v0, v1, v2, v3, normal));
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

            // new normal vector is average of four adjacent normal vectors
            polygons[i][j].new_normal = glm::normalize(norm_up + norm_down + norm_left + norm_right);
        }
    }

    for (unsigned i = 0; i < ii - 1; i++) {
        const unsigned c = surface.sections[i].points.size();
        for (unsigned j = 0; j < c; j++) {
            unsigned ip = (i + 1) - (i == ii - 2);
            unsigned jp = (j + c + 1) % c;
            // calculate normals
            polygons[i][j].normal = polygons[i][j].new_normal;
            polygons[i][j].r_normal = polygons[ip][j].new_normal;
            polygons[i][j].u_normal = polygons[i][jp].new_normal;
            polygons[i][j].ru_normal = polygons[ip][jp].new_normal;
        }
    }

    // draw all polygons
    for (auto& poly_vec: polygons) {
        for (auto& poly: poly_vec) {
            poly.draw();
        }
    }

    // draw translucent cube
    glm::vec3 camera_vec = glm::vec3(camera_pos[0], camera_pos[1], camera_pos[2]);
    glm::vec3 ver0 = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 ver1 = glm::vec3(7.5f, 0.0f, 0.0f);
    glm::vec3 ver2 = glm::vec3(7.5f, 7.5f, 0.0f);
    glm::vec3 ver3 = glm::vec3(0.0f, 7.5f, 0.0f);
    glm::vec3 n = glm::vec3(0.0f, 0.0f, 1.0f);

    std::vector<Polygon> cube_faces;
    for (int i = 0; i < 6; i++) {
        cube_faces.push_back(Polygon(ver0, ver1, ver2, ver3, n));
    }

    float pi = std::atan(1) * 4;
    cube_faces[0].rotate(0.0f, 0.0f, 1.0f, 0.0f);
    cube_faces[1].rotate(pi / 2, 0.0f, 1.0f, 0.0f);
    cube_faces[1].translate(7.5f, 0.0f, 0.0f);
    cube_faces[2].rotate(pi, 0.0f, 1.0f, 0.0f);
    cube_faces[2].translate(7.5f, 0.0f, -7.5f);
    cube_faces[3].rotate(pi / 2 * 3, 0.0f, 1.0f, 0.0f);
    cube_faces[3].translate(0.0f, 0.0f, -7.5f);
    cube_faces[4].rotate(pi / 2, -1.0f, 0.0f, 0.0f);
    cube_faces[4].translate(0.0f, 7.5f, 0.0f);
    cube_faces[5].rotate(pi / 2, 1.0f, 0.0f, 0.0f);
    cube_faces[5].translate(0.0f, 0.0f, -7.5f);

    for (int i = 0; i < 6; i++) {
        cube_faces[i].translate(0.0f, 15.0f, 0.0f);
    }

    // http://www.it.hiof.no/~borres/j3d/explain/light/p-materials.html
    // ruby
    cube_faces[0].ambient = glm::vec4(0.1745f, 0.01175f, 0.01175f, 0.55f);
    cube_faces[0].diffuse = glm::vec4(0.61424f, 0.04136f, 0.04136f, 0.55f);
    cube_faces[0].specular = glm::vec4(0.727811f, 0.626959f, 0.626959f, 0.55f);
    cube_faces[0].shininess = 76.8f;

    // gold
    cube_faces[1].ambient = glm::vec4(0.24725f, 0.1995f, 0.0745f, 0.5f);
    cube_faces[1].diffuse = glm::vec4(0.75164f, 0.60648f, 0.22648f, 0.5f);
    cube_faces[1].specular = glm::vec4(0.628281f, 0.555802f, 0.366065f, 0.5f);
    cube_faces[1].shininess = 51.2f;

    // silver
    cube_faces[2].ambient = glm::vec4(0.19225f, 0.19225f, 0.19225f, 0.5f);
    cube_faces[2].diffuse = glm::vec4(0.50754f, 0.50754f, 0.50754f, 0.5f);
    cube_faces[2].specular = glm::vec4(0.508273f, 0.508273f, 0.508273f, 0.5f);
    cube_faces[2].shininess = 51.2f;

    // chrome
    cube_faces[3].ambient = glm::vec4(0.25f, 0.25f, 0.25f, 0.5f);
    cube_faces[3].diffuse = glm::vec4(0.4f, 0.4f, 0.4f, 0.5f);
    cube_faces[3].specular = glm::vec4(0.774597f, 0.774597f, 0.774597f, 0.5f);
    cube_faces[3].shininess = 76.8f;

    // jade
    cube_faces[4].ambient = glm::vec4(0.135f, 0.2225f, 0.1575f, 0.5f);
    cube_faces[4].diffuse = glm::vec4(0.54f, 0.89f, 0.63f, 0.5f);
    cube_faces[4].specular = glm::vec4(0.316228f, 0.316228f, 0.316228f, 0.5f);
    cube_faces[4].shininess = 12.8f;

    // brass
    cube_faces[5].ambient = glm::vec4(0.329412f, 0.223529f, 0.027451f, 0.6f);
    cube_faces[5].diffuse = glm::vec4(0.780392f, 0.568627f, 0.113725f, 0.6f);
    cube_faces[5].specular = glm::vec4(0.992157f, 0.941176f, 0.807843f, 0.6f);
    cube_faces[5].shininess = 27.8974f;

    std::sort(cube_faces.begin(), cube_faces.end());

    for (int i = 0; i < 6; i++) {
        cube_faces[i].draw_cube_face();
    }

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

void init() {
    // enable lighting
    glEnable(GL_LIGHTING);
    GLfloat light_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

    GLfloat light0_pos[] = { 0.0f, 50.0f, 200.0f, 0.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, light0_pos);
    glEnable(GL_LIGHT0);

    GLfloat light1_pos[] = { 0.0f, -200.0f, 0.0f, 0.0f };
    glLightfv(GL_LIGHT1, GL_POSITION, light1_pos);
    glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
    // glEnable(GL_LIGHT1);

    GLfloat light2_pos[] = { 150.0f, 50.0f, 0.0f, 0.0f };
    glLightfv(GL_LIGHT2, GL_POSITION, light2_pos);
    glLightfv(GL_LIGHT2, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT2, GL_SPECULAR, light_specular);
    // glEnable(GL_LIGHT2);

    GLfloat light3_pos[] = { -150.0f, 50.0f, 0.0f, 0.0f };
    glLightfv(GL_LIGHT3, GL_POSITION, light3_pos);
    glLightfv(GL_LIGHT3, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT3, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT3, GL_SPECULAR, light_specular);
    // glEnable(GL_LIGHT3);

    // enable depth test
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_DEPTH_BUFFER_BIT);

    // enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glShadeModel(GL_SMOOTH);
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 800);
    glutInitWindowPosition(50, 0);
    glutCreateWindow("Homework 3");

    init();

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
