#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include "polygon.h"

Polygon::Polygon(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 normal, bool is_front) {
    points[0] = p0;
    points[1] = p1;
    points[2] = p2;
    points[3] = p3;
    this->normal = normal;
    this->is_front = is_front;
}

void Polygon::draw() {
    glBegin(GL_QUADS);
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
