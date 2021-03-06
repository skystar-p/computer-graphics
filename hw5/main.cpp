#include <glm/glm.hpp>
#include <vector>
#include <png.h>
#include "world.h"
#include "sphere.h"
#include "surface.h"
#include "image.h"
#include "parse.h"

#include <cstdio>

#define CAST(x) ((Object *)(x))


static const float view_width = 720.0f, view_height = 405.0f;
static const int width = 3600, height = 2000;
const glm::vec3 eye_init(0.0f, 0.0f, 300.0f);
glm::vec3 background(1.0f, 1.0f, 1.0f);

int main() {
    World world;

    // lights
    Light light1(glm::vec3(0.0f, 100.0f, 100.0f), 8000.0f);
    Light light2(glm::vec3(0.0f, 100.0f, -100.0f), 8000.0f);
    Light light3(glm::vec3(-100.0f, 30.0f, 200.0f), 4000.0f);
    world.lights.push_back(light1);
    world.lights.push_back(light2);
    world.lights.push_back(light3);

    Sphere sphere1(
            glm::vec3(-100.0f, 30.0f, 100.0f), 50.0f,
            glm::vec3(0.0f, 0.1f, 0.01f),
            glm::vec3(0.2f, 0.2f, 0.2f),
            glm::vec3(0.4f, 0.4f, 0.4f),
            3, 1.5f, false, false);

    Sphere sphere2(
            glm::vec3(0.0f, 0.0f, 50.0f), 50.0f,
            glm::vec3(0.1745f, 0.01175f, 0.01175f),
            glm::vec3(0.61424f, 0.04136f, 0.04136f),
            glm::vec3(0.727811f, 0.626959f, 0.626959f),
            3, 1.03f, true, true);

    (CAST(&sphere1))->texture = read_image((char *)"./assets/texture.png");
    (CAST(&sphere1))->bump_map = read_image((char *)"./assets/normal.png");

    (CAST(&sphere2))->reflect_coeff = 1.5f;
    (CAST(&sphere2))->refract_coeff = 0.4f;

    for (int i = 0; i < 11; i++) {
        // float yd = abs(5 - i) * 10.0f;
        float yd = (i - 5) * (i - 5) * 3.0f;
        Sphere *ss = new Sphere(
                glm::vec3(-200.0f + 40 * i, 120.0f - yd, 50.0f), 7.0f,
                glm::vec3(0.01f, 0.01f, 0.01f),
                glm::vec3(0.2f, 0.2f, 0.2f),
                glm::vec3(0.8f, 0.8f, 0.8f),
                3, 1.03f, true, true);
        world.objects.push_back(CAST(ss));
    }

    Triangle mirror1(
            glm::vec3(0.0f, 200.0f, -200.0f),
            glm::vec3(0.0f, -200.0f, -200.0f),
            glm::vec3(400.0f, -200.0f, 0.0f),
            glm::vec3(0.3f, 0.3f, 0.3f),
            glm::vec3(0.4f, 0.4f, 0.4f),
            glm::vec3(0.3f, 0.3f, 0.3f),
            3, 1.0f, true, false);

    Triangle mirror2(
            glm::vec3(400.0f, -200.0f, 0.0f),
            glm::vec3(400.0f, 200.0f, 0.0f),
            glm::vec3(0.0f, 200.0f, -200.0f),
            glm::vec3(0.3f, 0.3f, 0.3f),
            glm::vec3(0.4f, 0.4f, 0.4f),
            glm::vec3(0.3f, 0.3f, 0.3f),
            3, 1.0f, true, false);

    Triangle mirror3(
            glm::vec3(0.0f, 200.0f, -200.0f),
            glm::vec3(0.0f, -200.0f, -200.0f),
            glm::vec3(-400.0f, -200.0f, 0.0f),
            glm::vec3(0.3f, 0.3f, 0.3f),
            glm::vec3(0.4f, 0.4f, 0.4f),
            glm::vec3(0.3f, 0.3f, 0.3f),
            3, 1.0f, true, false);

    Triangle mirror4(
            glm::vec3(-400.0f, -200.0f, 0.0f),
            glm::vec3(-400.0f, 200.0f, 0.0f),
            glm::vec3(0.0f, 200.0f, -200.0f),
            glm::vec3(0.3f, 0.3f, 0.3f),
            glm::vec3(0.4f, 0.4f, 0.4f),
            glm::vec3(0.3f, 0.3f, 0.3f),
            3, 1.0f, true, false);

    (CAST(&mirror1))->reflect_coeff = 1.0f;
    (CAST(&mirror2))->reflect_coeff = 1.0f;
    (CAST(&mirror3))->reflect_coeff = 1.0f;
    (CAST(&mirror4))->reflect_coeff = 1.0f;

    Triangle floor1(
            glm::vec3(400.0f, -70.0f, -400.0f),
            glm::vec3(-400.0f, -70.0f, -400.0f),
            glm::vec3(400.0f, -70.0f, 400.0f),
            glm::vec3(0.0f, 0.01f, 0.0f),
            glm::vec3(0.4f, 0.4f, 0.4f),
            glm::vec3(0.7f, 0.7f, 0.7f),
            3, 1.0f, true, false);

    Triangle floor2(
            glm::vec3(-400.0f, -70.0f, -400.0f),
            glm::vec3(-400.0f, -70.0f, 400.0f),
            glm::vec3(400.0f, -70.0f, 400.0f),
            glm::vec3(0.0f, 0.01f, 0.0f),
            glm::vec3(0.4f, 0.4f, 0.4f),
            glm::vec3(0.7f, 0.7f, 0.7f),
            3, 1.0f, true, false);

    (CAST(&floor1))->reflect_coeff = 0.7f;
    (CAST(&floor2))->reflect_coeff = 0.7f;

    Surface surface1(
            glm::vec3(0.1f, 0.18725f, 0.1745f),
            glm::vec3(0.396f, 0.74151f, 0.69102f),
            glm::vec3(0.297254f, 0.30829f, 0.306678f),
            3, 1.0f, true, false);

    load_obj(surface1, (char *)"./assets/octahedron.obj");
    surface1.scale(30.0f);
    surface1.translate(glm::vec3(80.0f, -30.0f, 100.0f));

    Surface surface2(
            glm::vec3(0.0f, 0.1f, 0.0f),
            glm::vec3(0.2f, 0.2f, 0.2f),
            glm::vec3(0.5f, 0.5f, 0.5f),
            3, 1.0f, false, false);

    load_obj(surface2, (char *)"./assets/octahedron.obj");
    surface2.scale(25.0f);
    surface2.translate(glm::vec3(10.0f, -30.0f, -20.0f));

    world.objects.push_back(CAST(&sphere1));
    world.objects.push_back(CAST(&sphere2));
    world.objects.push_back(CAST(&mirror1));
    world.objects.push_back(CAST(&mirror2));
    world.objects.push_back(CAST(&mirror3));
    world.objects.push_back(CAST(&mirror4));
    world.objects.push_back(CAST(&floor1));
    world.objects.push_back(CAST(&floor2));
    for (int i = 0; i < (int) surface1.triangles.size(); i++) {
        world.objects.push_back(CAST(&surface1.triangles[i]));
    }
    for (int i = 0; i < (int) surface2.triangles.size(); i++) {
        world.objects.push_back(CAST(&surface2.triangles[i]));
    }

    std::vector<glm::vec3> data;
    data = world.render(view_width, view_height, width, height);

    write_image(data, width, height, (char *)"./result.png");
}
