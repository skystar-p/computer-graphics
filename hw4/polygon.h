#ifndef POLYGON_H
#define POLYGON_H

#include <glm/glm.hpp>
#include <vector>

extern GLfloat camera_pos[3];

class Polygon {
    public:
        Polygon(glm::vec3, glm::vec3, glm::vec3, glm::vec3, glm::vec3);
        void draw();

        glm::vec3 points[4];
        glm::vec3 normal;
        glm::vec3 r_normal;
        glm::vec3 ru_normal;
        glm::vec3 u_normal;
        glm::vec3 new_normal;
};

#endif
