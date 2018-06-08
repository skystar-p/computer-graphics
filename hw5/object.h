#ifndef OBJECT_H
#define OBJECT_H

#include <glm/glm.hpp>
#include "ray.h"
#include "image.h"

class Object {
    public:
        Object(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
                int gloss, float n, bool is_reflective,
                bool is_refractive) :
            gloss(gloss), is_reflective(is_reflective),
            is_refractive(is_refractive), n(n),
            ambient(ambient), diffuse(diffuse), specular(specular) {};
        virtual Ray reflect(Ray) = 0;
        virtual Ray refract(Ray) = 0;
        virtual bool has_intersection(Ray) = 0;
        virtual glm::vec3 intersect(Ray) = 0;
        virtual glm::vec3 normal(glm::vec3) = 0;
        virtual glm::vec3 get_texture_pixel(glm::vec3) = 0;

        const int gloss;
        const bool is_reflective, is_refractive;
        const float n;

        float reflect_coeff = 1.0f, refract_coeff = 1.0f;

        glm::vec3 ambient, diffuse, specular;

        image_info_t *texture = NULL;
        image_info_t *bump_map = NULL;
};

#endif
