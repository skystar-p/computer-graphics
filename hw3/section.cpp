#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <vector>
#include "section.h"
#include "spline.h"
#include "spline_type.h"

extern int SPLINE_DIVISION;


Section::Section(
        std::vector<glm::vec3> control_points,
        SplineType spline_type,
        float scale,
        glm::quat rotation,
        glm::vec3 translate) {

    this->spline_type = spline_type;
    this->scale = scale;
    this->rotation = rotation;
    this->translate = translate;

    for (auto point: control_points) {
        // follow the transformation order of file format
        point = point * scale;
        point = glm::vec3(glm::toMat4(rotation) * glm::vec4(point, 1.0f));
        point += translate;
    }

    int c = control_points.size();

    for (unsigned i = 0; i < control_points.size(); i++) {
        std::vector<glm::vec3> int_points;
        for (int ii = 0; ii < 4; ii++) {
            int_points.push_back(control_points[(i + ii) % c]);
        }

        if (spline_type == SplineType::CatmullRomSpline) {
            for (int j = 0; j < SPLINE_DIVISION; j++) {
                float param = (float) j / (float) SPLINE_DIVISION;
                glm::vec3 p = catmullrom_spline(
                        int_points[1],
                        int_points[2],
                        (int_points[2] - int_points[0]) / 2.0f,
                        (int_points[3] - int_points[1]) / 2.0f,
                        param);
                points.push_back(p);
            }
        }
        else { // b-spline
            for (int j = 0; j < SPLINE_DIVISION; j++) {
                float param = (float) j / (float) SPLINE_DIVISION;
                glm::vec3 p = b_spline(
                        int_points[0],
                        int_points[1],
                        int_points[2],
                        int_points[3],
                        param);
                points.push_back(p);
            }
        }

    }
}
