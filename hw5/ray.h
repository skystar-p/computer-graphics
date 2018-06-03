#ifndef RAY_H
#define RAY_H

#include <glm/glm.hpp>

class Ray {
    public:
        Ray(glm::vec3 o, glm::vec3 d, float n_)
            : origin(o), direction(glm::normalize(d)), n(n_) {};
        glm::vec3 origin;
        glm::vec3 direction;
        float n;
};

#endif
