#ifndef SECTION_H
#define SECTION_H

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <vector>
#include "spline_type.h"

class Section {
    public:
        Section(std::vector<glm::vec3>, SplineType, float, glm::quat, glm::vec3);

        void calculate();

        std::vector<glm::vec3> points;
        SplineType spline_type;
        glm::quat rotation;
        glm::vec3 translate;
        float scale;
};

#endif
