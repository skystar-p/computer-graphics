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
    this->points = control_points;

}

void Section::calculate() {
    std::vector<glm::vec3> transformed_control_points;
    for (unsigned i = 0; i < points.size(); i++) {
        glm::vec3 point = glm::vec3(points[i].x, points[i].y, points[i].z);
        point = point * scale;
        point = glm::vec3(glm::toMat4(rotation) * glm::vec4(point, 1.0f));
        point += translate;
        transformed_control_points.push_back(point);
    }

    int c = transformed_control_points.size();

    std::vector<glm::vec3> result;

    for (unsigned i = 0; i < transformed_control_points.size(); i++) {
        std::vector<glm::vec3> int_points;
        for (int ii = 0; ii < 4; ii++) {
            int_points.push_back(transformed_control_points[(i + ii) % c]);
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
                result.push_back(p);
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
                result.push_back(p);
            }
        }
    }

    points = result;
}
