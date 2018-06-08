#include <glm/glm.hpp>
#include <vector>
#include <png.h>
#include "world.h"
#include "sphere.h"
#include "surface.h"
#include "image.h"

#include <cstdio>

#define CAST(x) ((Object *)(x))

#define OUTPUT (char *)"./result.png"

static const float view_width = 720.0f, view_height = 400.0f;
static const int width = 1800 / 3, height = 1000 / 3;
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
            glm::vec3(0.0f, 0.15f, 0.15f),
            glm::vec3(0.2f, 0.2f, 0.2f),
            glm::vec3(0.7f, 0.7f, 0.7f),
            3, 1.03f, true, true);

    (CAST(&sphere1))->texture = read_image((char *)"./assets/texture.png");
    (CAST(&sphere1))->bump_map = read_image((char *)"./assets/normal.png");

    (CAST(&sphere2))->reflect_coeff = 1.5f;
    (CAST(&sphere2))->refract_coeff = 0.4f;

    Triangle t1(
            glm::vec3(0.0f, 200.0f, -200.0f),
            glm::vec3(0.0f, -200.0f, -200.0f),
            glm::vec3(200.0f, -200.0f, 0.0f),
            glm::vec3(0.3f, 0.3f, 0.3f),
            glm::vec3(0.4f, 0.4f, 0.4f),
            glm::vec3(0.3f, 0.3f, 0.3f),
            3, 1.5f, true, false);

    Triangle t2(
            glm::vec3(200.0f, -200.0f, 0.0f),
            glm::vec3(200.0f, 200.0f, 0.0f),
            glm::vec3(0.0f, 200.0f, -200.0f),
            glm::vec3(0.3f, 0.3f, 0.3f),
            glm::vec3(0.4f, 0.4f, 0.4f),
            glm::vec3(0.3f, 0.3f, 0.3f),
            3, 1.5f, true, false);

    Triangle floor1(
            glm::vec3(200.0f, -70.0f, -200.0f),
            glm::vec3(-200.0f, -70.0f, -200.0f),
            glm::vec3(200.0f, -70.0f, 200.0f),
            glm::vec3(0.5f, 0.0f, 0.0f),
            glm::vec3(0.4f, 0.4f, 0.4f),
            glm::vec3(0.7f, 0.7f, 0.7f),
            3, 1.5f, false, false);

    Triangle floor2(
            glm::vec3(-200.0f, -70.0f, -200.0f),
            glm::vec3(-200.0f, -70.0f, 200.0f),
            glm::vec3(200.0f, -70.0f, 200.0f),
            glm::vec3(0.5f, 0.0f, 0.0f),
            glm::vec3(0.4f, 0.4f, 0.4f),
            glm::vec3(0.7f, 0.7f, 0.7f),
            3, 1.5f, false, false);

    world.objects.push_back(CAST(&sphere1));
    world.objects.push_back(CAST(&sphere2));
    world.objects.push_back(CAST(&t1));
    world.objects.push_back(CAST(&t2));
    world.objects.push_back(CAST(&floor1));
    world.objects.push_back(CAST(&floor2));
    (CAST(&t1))->reflect_coeff = 1.0f;
    (CAST(&t2))->reflect_coeff = 1.0f;

    std::vector<glm::vec3> data;
    data = world.render(view_width, view_height, width, height);

    write_image(data, width, height, OUTPUT);
}
