#include <glm/glm.hpp>
#include <vector>
#include "spline.h"
#include "surface.h"

extern int SPLINE_DIVISION;


Surface::Surface(std::vector<Section> cross_sections, SplineType spline_type) {
    for (unsigned i = 0; i < cross_sections.size() - 1; i++) {
        Section cs1 = cross_sections[i];
        Section cs2 = cross_sections[i + 1];

        // interpolate scale
        float scale_tangent1 = (i == 0) ?
            (cs2.scale - cs1.scale) / 2.0f : (cs2.scale - cross_sections[i - 1].scale) / 2.0f;
        float scale_tangent2 = (i == cross_sections.size() - 2) ?
            (cs2.scale - cs1.scale) / 2.0f : (cross_sections[i + 2].scale - cs1.scale) / 2.0f;

        // interpolate rotation
        glm::quat rotation_tangent1 = (i == 0) ?
            (glm::inverse(cs1.rotation) * cs2.rotation) / 2.0f
            : (glm::inverse(cross_sections[i - 1].rotation) * cs2.rotation) / 2.0f;
        glm::quat rotation_tangent2 = (i == 0) ?
            (glm::inverse(cs1.rotation) * cs2.rotation) / 2.0f
            : (glm::inverse(cs1.rotation) * cross_sections[i + 2].rotation) / 2.0f;

        // interpolate translation
        glm::vec3 translate_tangent1 = (i == 0) ?
            (cs2.translate - cs1.translate) / 2.0f : (cs2.translate - cross_sections[i - 1].translate) / 2.0f;
        glm::vec3 translate_tangent2 = (i == cross_sections.size() - 2) ?
            (cs2.translate - cs1.translate) / 2.0f : (cross_sections[i + 2].translate - cs1.translate) / 2.0f;

        for (int j = 0; j < SPLINE_DIVISION; j++) {
            float t = (float) j / (float) SPLINE_DIVISION;

            float int_scale = catmullrom_spline(cs1.scale, cs2.scale, scale_tangent1, scale_tangent2, t);
            glm::quat int_rotation = catmullrom_spline(cs1.rotation, cs2.rotation, rotation_tangent1, rotation_tangent2, t);
            glm::vec3 int_translate = catmullrom_spline(cs1.translate, cs2.translate, translate_tangent1, translate_tangent2, t);

            // interpolate control points
            assert(cs1.points.size() == cs2.points.size());
            std::vector<glm::vec3> int_control_points;
            for (unsigned k = 0; k < cs1.points.size(); k++) {
                glm::vec3 point_tangent1 = (i == 0) ?
                    (cs2.points[k] - cs1.points[k]) / 2.0f : (cs2.points[k] - cross_sections[i - 1].points[k]) / 2.0f;
                glm::vec3 point_tangent2 = (i == cross_sections.size() - 2) ?
                    (cs2.points[k] - cs1.points[k]) / 2.0f : (cross_sections[i + 2].points[k] - cs1.points[k]) / 2.0f;
                glm::vec3 int_point = catmullrom_spline(cs1.points[k], cs2.points[k], point_tangent1, point_tangent2, t);

                int_control_points.push_back(int_point);
            }

            // build new subsections
            Section new_section(
                    int_control_points,
                    spline_type,
                    int_scale,
                    int_rotation,
                    int_translate);
            sections.push_back(new_section);
        }

        // construct average normal vectors
        for (unsigned i = 0; i < sections.size(); i++) {
            unsigned c = sections[i].points.size();
            for (unsigned j = 0; j < c; j++) {
                glm::vec3 v_left = (j == 0) ? sections[i].points[c - 1] : sections[i].points[j - 1];
                glm::vec3 v_right = (j == c - 1) ? sections[i].points[0] : sections[i].points[j + 1];
                glm::vec3 v_up = (i == sections.size() - 1) ? sections[i].points[j] : sections[i + 1].points[j];
                glm::vec3 v_down = (i == 0) ? sections[i].points[j] : sections[i - 1].points[j];

                normals.push_back(glm::cross(v_right - v_left, v_up - v_down));
            }
        }
    }
}
