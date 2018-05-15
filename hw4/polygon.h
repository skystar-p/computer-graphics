#ifndef POLYGON_H
#define POLYGON_H

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <vector>

extern GLfloat camera_pos[3];

class Polygon {
    public:
        Polygon(glm::vec3, glm::vec3, glm::vec3, glm::vec3, glm::vec3);
        void draw();
        void draw_cube_face();
        void rotate(float, float, float, float);
        void translate(float, float, float);
        bool operator< (const Polygon& p) {
            glm::vec3 c(camera_pos[0], camera_pos[1], camera_pos[2]);
            glm::vec3 my_center =
                (points[0] + points[1] + points[2] + points[3]) / 4.0f;
            glm::vec3 others_center =
                (p.points[0] + p.points[1] + p.points[2] + p.points[3]) / 4.0f;

            return glm::length(my_center - c) > glm::length(others_center - c);
        }

        glm::vec3 points[4];
        glm::vec3 normal, r_normal, ru_normal, u_normal, new_normal;
        glm::vec4 color;
};

#endif
