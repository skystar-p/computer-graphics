#include <glm/glm.hpp>
#include <vector>
#include <png.h>
#include "image.h"

void write_image(std::vector<glm::vec3> data, int width, int height, char * file_path) {
    FILE *f = fopen(file_path, "wb");
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
