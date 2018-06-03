#ifndef OBJECT_H
#define OBJECT_H

#include <glm/glm.hpp>
#include "ray.h"

class Object {
    public:
        virtual Ray reflect(Ray) = 0;
        virtual Ray refract(Ray) = 0;
        virtual bool has_intersection(Ray) = 0;
        virtual glm::vec3 intersect(Ray) = 0;
        virtual glm::vec3 normal(glm::vec3) = 0;

        int gloss;
        float n;
};

#endif
