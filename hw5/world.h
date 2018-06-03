#ifndef WORLD_H
#define WORLD_H

#include <glm/glm.hpp>
#include "ray.h"
#include "object.h"

class Light {
    public:
        Light(glm::vec3 pos, float inten) : position(pos), intensity(inten) {};
        glm::vec3 position;
        float intensity;
};

class World {
    public:
        glm::vec3 trace(Ray);
        glm::vec3 phong(glm::vec3);
};


#endif
