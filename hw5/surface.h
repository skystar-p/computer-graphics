#ifndef SURFACE_H
#define SURFACE_H

#include <glm/glm.hpp>
#include <vector>
#include "object.h"

class Quad : Object {
    public:
        Quad(glm::vec3, glm::vec3, glm::vec3, glm::vec3);

        bool includes(glm::vec3);

        std::vector<glm::vec3> points;
        glm::vec3 vn;
};

class Surface : Object {
    public:
        Ray reflect(Ray);
        Ray refract(Ray);
        bool has_intersection(Ray);
        glm::vec3 intersect(Ray);
        glm::vec3 normal(glm::vec3);

        std::vector<Quad> quads;
};

#endif
