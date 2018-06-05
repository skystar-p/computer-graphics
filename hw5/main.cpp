#include <glm/glm.hpp>
#include <vector>
#include <png.h>
#include "world.h"
#include "sphere.h"
#include "surface.h"

#include <cstdio>

#define OUTPUT "./result.png"

void write_image(std::vector<glm::vec3>);
png_byte norm(float);

static const float view_width = 720.0f, view_height = 400.0f;
static const int width = 1800 / 3, height = 1000 / 3;
const glm::vec3 eye_init(0.0f, 0.0f, 200.0f);
glm::vec3 background(1.0f, 1.0f, 1.0f);

int main() {
    World world;

    // lights
    Light light1(glm::vec3(0.0f, 100.0f, 100.0f), 8000.0f);
    Light light2(glm::vec3(0.0f, 100.0f, -200.0f), 8000.0f);
    world.lights.push_back(light1);
    world.lights.push_back(light2);

    Sphere sphere1(
            glm::vec3(0.0f, 20.0f, 0.0f), 50.0f,
            glm::vec3(0.0f, 0.8f, 0.2f),
            glm::vec3(0.3f, 0.3f, 0.3f),
            glm::vec3(0.2f, 0.2f, 0.2f),
            3, 1.5f, false, false);

    Triangle t1(
            glm::vec3(300.0f, 300.0f, 0.0f),
            glm::vec3(300.0f, -300.0f, 0.0f),
            glm::vec3(150.0f, -300.0f, -150.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            3, 1.5f, true, false);

    Triangle t2(
            glm::vec3(300.0f, 300.0f, 0.0f),
            glm::vec3(150.0f, -300.0f, -150.0f),
            glm::vec3(150.0f, 300.0f, -150.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            3, 1.5f, true, false);

    world.objects.push_back((Object *)&sphere1);
    world.objects.push_back((Object *)&t1);
    world.objects.push_back((Object *)&t2);
    ((Object *)&t1)->reflect_coeff = 100.0f;
    ((Object *)&t2)->reflect_coeff = 100.0f;

    std::vector<glm::vec3> data;
    data = world.render(view_width, view_height, width, height);

    write_image(data);
}

void write_image(std::vector<glm::vec3> data) {
    FILE *f = fopen(OUTPUT, "wb");
    png_structp png_struct = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    png_infop png_info = png_create_info_struct(png_struct);
    setjmp(png_jmpbuf(png_struct));

    // initialize io
    png_init_io(png_struct, f);

    // set png header
    png_set_IHDR(png_struct, png_info, width, height, 8,
            PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

    // write title text
    png_text png_title;
    png_title.compression = PNG_TEXT_COMPRESSION_NONE;
    png_title.key = (png_charp) "raytrace";
    png_title.text = (png_charp) "raytrace";

    // write to the struct
    png_set_text(png_struct, png_info, &png_title, 1);

    // write all information to file
    png_write_info(png_struct, png_info);

    png_byte *image = (png_byte *) malloc(sizeof(png_byte) * 3 * width * height);

    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            int ind = (i * height + j);
            image[ind * 3] = norm(data[ind].x);
            image[ind * 3 + 1] = norm(data[ind].y);
            image[ind * 3 + 2] = norm(data[ind].z);
        }
    }

    for (int i = 0; i < height; i++) {
        png_write_row(png_struct, &image[i * width * 3]);
    }

    png_write_end(png_struct, NULL);

    free(image);
}

png_byte norm(float v) {
    v *= 255.0f;
    if (v < 0.0f) {
        return 0;
    }

    if (v > 255.0f) {
        return 255;
    }

    return (png_byte) v;
}
