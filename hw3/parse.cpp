#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <fstream>
#include <vector>
#include "parse.h"
#include "spline_type.h"


#include <cstdio>
Surface read_from_file(std::string path) {
    std::ifstream fin(path, std::ios::in);

    std::string spline_type_in;
    SplineType spline_type;
    fin >> spline_type_in;

    if (spline_type_in == "BSPLINE") {
        spline_type = SplineType::BSpline;
    }
    else {
        assert(spline_type_in == "CATMULL_ROM");
        spline_type = SplineType::CatmullRomSpline;
    }

    int cross_section_count, control_point_count;
    fin >> cross_section_count >> control_point_count;

    std::vector<Section> cross_sections;
    for (int i = 0; i < cross_section_count; i++) {
        double x, y, z;
        double scale, angle, rx, ry, rz;
        double tx, ty, tz;

        std::vector<glm::vec3> control_points;
        for (int j = 0; j < control_point_count; j++) {
            y = 0.0;
            fin >> x >> z;
            control_points.push_back(glm::vec3((float) x, (float) y, (float) z));
        }
        fin >> scale >> angle >> rx >> ry >> rz >> tx >> ty >> tz;

        glm::quat rotation(
                cos(angle / 2.0),
                rx * sin(angle / 2.0),
                ry * sin(angle / 2.0),
                rz * sin(angle / 2.0));

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
