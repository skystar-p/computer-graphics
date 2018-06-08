#include <glm/glm.hpp>
#include <vector>
#include <fstream>
#include "surface.h"
#include "parse.h"

#include <iostream>


void load_obj(Surface &surface, char *file_path) {
    std::ifstream fin(file_path, std::ios::in);
    std::vector<glm::vec3> vertices;
    Object *s = (Object *)&surface;

    std::string k;
    double a, b, c;
    int v1, v2, v3;
    while (true) {
        fin >> k;
        if (fin.eof()) {
            break;
        }

        if (k == "v") {
            fin >> a >> b >> c;
            vertices.push_back(glm::vec3((float) a, (float) b, (float) c));
        }

        if (k == "f") {
            fin >> v1 >> v2 >> v3;
            v1 -= 1; v2 -= 1; v3 -= 1;
            Triangle t(
                    vertices[v1], vertices[v2], vertices[v3],
                    s->ambient, s->diffuse, s->specular, s->gloss,
                    s->n, s->is_reflective, s->is_refractive);
            surface.triangles.push_back(t);
        }
    }
}
