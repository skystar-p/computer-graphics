#ifndef IMAGE_H
#define IMAGE_H

#include <glm/glm.hpp>
#include <vector>
#include <png.h>

typedef struct image_info {
    png_bytep *data;
    int width;
    int height;
} image_info_t;

image_info_t *read_image(char *);
void write_image(std::vector<glm::vec3>, int, int, char *);

#endif
