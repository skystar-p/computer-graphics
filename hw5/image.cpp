#include <glm/glm.hpp>
#include <vector>
#include <png.h>
#include "image.h"

png_byte norm(float);

image_info_t *read_image(char *file_path) {

    FILE *f = fopen(file_path, "rb");

    png_structp png_struct = png_create_read_struct(
            PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

    png_infop png_info = png_create_info_struct(png_struct);

    setjmp(png_jmpbuf(png_struct));

    png_init_io(png_struct, f);

    png_read_info(png_struct, png_info);

    int width = png_get_image_width(png_struct, png_info);
    int height = png_get_image_height(png_struct, png_info);
    png_byte color_type = png_get_color_type(png_struct, png_info);
    png_byte bit_depth = png_get_bit_depth(png_struct, png_info);

    if (bit_depth == 16) {
        png_set_strip_16(png_struct);
    }

    if (color_type == PNG_COLOR_TYPE_PALETTE) {
        png_set_expand_gray_1_2_4_to_8(png_struct);
    }

    if (png_get_valid(png_struct, png_info, PNG_INFO_tRNS)) {
        png_set_tRNS_to_alpha(png_struct);
    }

    if (color_type == PNG_COLOR_TYPE_RGB ||
            color_type == PNG_COLOR_TYPE_GRAY ||
            color_type == PNG_COLOR_TYPE_PALETTE) {
        png_set_filler(png_struct, 0xFF, PNG_FILLER_AFTER);
    }

    if (color_type == PNG_COLOR_TYPE_GRAY ||
            color_type == PNG_COLOR_TYPE_GRAY_ALPHA) {
        png_set_gray_to_rgb(png_struct);
    }

    png_read_update_info(png_struct, png_info);

    png_bytep *image_row = (png_bytep *) malloc(sizeof(png_bytep) * height);

    for (int i = 0; i < height; i++) {
        image_row[i] =
            (png_byte *) malloc(png_get_rowbytes(png_struct, png_info));
    }

    png_read_image(png_struct, image_row);

    fclose(f);

    image_info_t *result = (image_info_t *) malloc(sizeof(image_info_t));

    result->data = image_row;
    result->width = width;
    result->height = height;

    return result;
}

void write_image(std::vector<glm::vec3> data, int width, int height, char * file_path) {
    FILE *f = fopen(file_path, "wb");
    png_structp png_struct = png_create_write_struct(
            PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

    png_infop png_info = png_create_info_struct(png_struct);

    setjmp(png_jmpbuf(png_struct));

    // initialize io
    png_init_io(png_struct, f);

    // set png header
    png_set_IHDR(png_struct, png_info, width, height, 8,
            PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
            PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

    // write title text
    png_text png_title;
    png_title.compression = PNG_TEXT_COMPRESSION_NONE;
    png_title.key = (png_charp) "raytrace";
    png_title.text = (png_charp) "raytrace";

    // write to the struct
    png_set_text(png_struct, png_info, &png_title, 1);

    // write all information to file
    png_write_info(png_struct, png_info);

    png_byte *image = (png_byte *) malloc(
            sizeof(png_byte) * 3 * width * height);

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
    fclose(f);
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
