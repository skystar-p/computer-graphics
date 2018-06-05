#ifndef WORLD_H
#define WORLD_H

#include <glm/glm.hpp>
#include <vector>
#include "ray.h"
#include "object.h"

extern const glm::vec3 eye_init;

class Light {
    public:
        Light(glm::vec3 pos, float inten)
            : position(pos), intensity(inten) {};
        glm::vec3 position;
        float intensity;
};

class World {
    public:
        std::vector<glm::vec3> render(float, float, int, int);
        glm::vec3 trace(Ray, int);
        glm::vec3 get_color(Object *, glm::vec3);
        bool is_reachable(Light, glm::vec3);

        std::vector<Light> lights;
        std::vector<Object *> objects;
        const glm::vec3 eye = eye_init;
};

#endif
