#ifndef SPHERE_H
#define SPHERE_H

#include <glm/glm.hpp>
#include "object.h"

class Sphere : Object {
    public:
        Sphere(glm::vec3, float);

        glm::vec3 center;
        float radius;

        Ray reflect(Ray);
        Ray refract(Ray);
        bool has_intersection(Ray);
        glm::vec3 intersect(Ray);
        glm::vec3 normal(glm::vec3);
};
#endif
