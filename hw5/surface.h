#ifndef SURFACE_H
#define SURFACE_H

#include <glm/glm.hpp>
#include <vector>
#include "object.h"

class Triangle : Object {
    public:
        Triangle(glm::vec3 a, glm::vec3 b, glm::vec3 c,
                glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
                int gloss, float n, bool is_reflective,
                bool is_refractive);

        bool includes(glm::vec3);
        bool has_intersection(Ray);
        glm::vec3 intersect(Ray);

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

        std::vector<Triangle> triangles;
};

#endif
