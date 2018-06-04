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

static const int width = 180, height = 100;

int main() {
    World world;
    Light light1(glm::vec3(0.0f, 50.0f, -30.0f), 0.5f);
    Sphere sphere1(
            glm::vec3(0.0f, 0.0f, 30.0f), 10.0f,
            glm::vec3(0.1f, 0.1f, 0.1f),
            glm::vec3(0.2f, 0.3f, 0.4f),
            glm::vec3(0.5f, 0.3f, 0.5f),
            3, 0.7f, true, true);

    world.objects.push_back((Object *)&sphere1);
    world.lights.push_back(light1);

    std::vector<glm::vec3> data;
    data = world.render(180.0f, 100.0f, width, height);

    // for (const auto &d: data) {
    //     if (d.x < 250.0f)
    //         printf("%f %f %f\n", d.x, d.y, d.z);
    // }
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

    printf("WOWO\n");
    for (int i = 0; i < height; i++) {
        png_write_row(png_struct, &image[i * width * 3]);
    }

    png_write_end(png_struct, NULL);
    // png_write_info(png_struct, png_info);

    free(image);
}

png_byte norm(float v) {
    if (v < 0.0f) {
        return 0;
    }

    if (v > 255.0f) {
        return 255;
    }

    return (png_byte) v;
}
