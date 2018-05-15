#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include "polygon.h"

extern GLfloat camera_pos[3];

Polygon::Polygon(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 normal) {
    points[0] = p0;
    points[1] = p1;
    points[2] = p2;
    points[3] = p3;
    this->normal = normal;
}

void Polygon::draw() {
    glBegin(GL_QUADS);
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        glNormal3f(normal.x, normal.y, normal.z);
        glVertex3f(points[0].x, points[0].y, points[0].z);

        glNormal3f(r_normal.x, r_normal.y, r_normal.z);
        glVertex3f(points[1].x, points[1].y, points[1].z);

        glNormal3f(ru_normal.x, ru_normal.y, ru_normal.z);
        glVertex3f(points[2].x, points[2].y, points[2].z);

        glNormal3f(u_normal.x, u_normal.y, u_normal.z);
        glVertex3f(points[3].x, points[3].y, points[3].z);
    glEnd();
}

void Polygon::draw_cube_face() {
    glBegin(GL_QUADS);
        glColor4f(color[0], color[1], color[2], color[3]);
        glNormal3f(normal.x, normal.y, normal.z);
        glVertex3f(points[0].x, points[0].y, points[0].z);
        glVertex3f(points[1].x, points[1].y, points[1].z);
        glVertex3f(points[2].x, points[2].y, points[2].z);
        glVertex3f(points[3].x, points[3].y, points[3].z);
    glEnd();
}

void Polygon::rotate(float angle, float x, float y, float z) {
    glm::quat q(
            cos(angle / 2.0f),
            x * sin(angle / 2.0f),
            y * sin(angle / 2.0f),
            z * sin(angle / 2.0f)
            );
    for (int i = 0; i < 4; i++) {
        points[i] = glm::vec3(glm::toMat4(q) * glm::vec4(points[i], 0));
    }

    normal = glm::vec3(glm::toMat4(q) * glm::vec4(normal, 1));
}

void Polygon::translate(float x, float y, float z) {
    for (int i = 0; i < 4; i++) {
        points[i] += glm::vec3(x, y, z);
    }
}
