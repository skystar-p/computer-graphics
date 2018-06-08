#ifndef IMAGE_H
#define IMAGE_H

#include <glm/glm.hpp>
#include <vector>
#include <png.h>

void write_image(std::vector<glm::vec3>, int, int, char *);
png_byte norm(float);

#endif
