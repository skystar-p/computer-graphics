#ifndef SURFACE_H
#define SURFACE_H

#include <glm/glm.hpp>
#include <vector>
#include "spline_type.h"
#include "section.h"

class Surface {
    public:
        Surface(std::vector<Section>, SplineType);
        std::vector<Section> sections;
        std::vector<glm::vec3> normals;
};

#endif
