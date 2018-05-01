#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include "parse.h"
#include "spline_type.h"


Surface read_from_file(std::string path) {
    std::ifstream fin(path, std::ios::in);

    std::string spline_type_in;
    SplineType spline_type;
    fin >> spline_type_in;

    if (spline_type_in == "BSPLINE") {
        spline_type = SplineType::BSpline;
    }
    else {
        spline_type = SplineType::CatmullRomSpline;
    }

    int cross_section_count, control_point_count;
    fin >> cross_section_count >> control_point_count;

    std::vector<Section> cross_sections;
    for (int i = 0; i < cross_section_count; i++) {
        int x, y, z;
        float scale, angle, rx, ry, rz;
        int tx, ty, tz;

        std::vector<glm::vec3> control_points;
        for (int j = 0; j < control_point_count; j++) {
            y = 0.0f;
            fin >> x >> z;
            control_points.push_back(glm::vec3(x, y, z));
        }
        fin >> scale >> angle >> rx >> ry >> rz >> tx >> ty >> tz;

        glm::quat rotation(
                glm::cos(angle / 2.0f),
                rx * glm::sin(angle / 2.0f),
                ry * glm::sin(angle / 2.0f),
                rz * glm::sin(angle / 2.0f));
        glm::vec3 translation(tx, ty, tz);

        cross_sections.push_back(Section(
                    control_points,
                    spline_type,
                    scale,
                    rotation,
                    translation));
    }

    fin.close();

    Surface surface(cross_sections, spline_type);
    return surface;
}
