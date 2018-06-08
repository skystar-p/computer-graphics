#include "sphere.h"
#include "object.h"
#include "ray.h"
#include <cmath>

#include <cstdio>
#include <iostream>

#define EPSILON 1e-3f


Sphere::Sphere(glm::vec3 center, float radius, glm::vec3 ambient,
        glm::vec3 diffuse, glm::vec3 specular, int gloss,
        float n, bool is_reflective, bool is_refractive)
    : Object(ambient, diffuse, specular, gloss, n,
            is_reflective, is_refractive) {
        this->center = center;
        this->radius = radius;
}

bool Sphere::has_intersection(Ray ray) {
    glm::vec3 delta = center - ray.origin;
    float t = glm::dot(delta, ray.direction);
    float l = glm::length(delta);
    float d = t * t - (l * l - radius * radius);
    if (d < EPSILON || std::isnan(d)) {
        return false;
    }

    float s = t - sqrtf(d);
    if (s < EPSILON || std::isnan(s)) {
        return false;
    }
    return true;
}

glm::vec3 Sphere::intersect(Ray ray) {
    glm::vec3 delta = center - ray.origin;
    float t = glm::dot(delta, ray.direction);
    float l = glm::length(delta);
    float d = t * t - (l * l - radius * radius);
    // use Sphere::has_intersection to verify validity of this section
    float s = t - sqrtf(d);

    // check if s is smaller than EPSILON?
    return ray.origin + s * ray.direction;
}

Ray Sphere::reflect(Ray ray) {
    // always check true of has_intersection
    glm::vec3 intersection = intersect(ray);
    glm::vec3 norm = normal(intersection);
    glm::vec3 l = -ray.direction;

    float n = this->n;

    if (glm::dot(norm, l) < -EPSILON) {
        norm = -norm;
        n = ray.n;
    }

    glm::vec3 r = 2.0f * glm::dot(l, norm) * norm - l;

    return Ray(intersection, glm::normalize(r), ray.n);
}

Ray Sphere::refract(Ray ray) {
    glm::vec3 intersection = intersect(ray);
    glm::vec3 norm = normal(intersection);
    glm::vec3 l = -glm::normalize(ray.direction);
    // return Ray(intersection + EPSILON * ray.direction, ray.direction, n);

    float n = this->n;

    if (glm::dot(norm, l) < -EPSILON) {
        norm = -norm;
        n = ray.n;
    }

    float cos_i = glm::dot(l, norm);
    float cos_r =
        sqrtf(1.0f - (ray.n / n) * (ray.n / n) * (1.0f - cos_i * cos_i));
    glm::vec3 t = ((ray.n / n) * cos_i - cos_r) * norm - (ray.n / n) * l;

    return Ray(intersection, glm::normalize(t), n);
}

glm::vec3 Sphere::normal(glm::vec3 point) {
    if (bump_map == NULL) {
        return glm::normalize(point - center);
    }

    glm::vec3 twiddle = get_bump_map_pixel(point) * 2.0f - glm::vec3(1.0f);
    glm::vec3 z = glm::normalize(point - center);
    if (glm::length(glm::vec3(0.0f, 1.0f, 0.0f) - z) < EPSILON) {
        return z;
    }

    glm::vec3 x = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), z));
    glm::vec3 y = glm::normalize(glm::cross(z, x));


    glm::vec3 result = x * twiddle.x + y * twiddle.y + z * twiddle.z;

    return glm::normalize(result);
}

glm::vec3 Sphere::get_texture_pixel(glm::vec3 p) {
    if (texture == NULL) {
        // this should not be happened
        assert(false);
    }

    int width = texture->width;
    int height = texture->height;
    png_bytep *data = texture->data;
    
    glm::vec3 n = glm::normalize(p - center);
    float pi = std::atan(1) * 4;
    int u = (int) (((float) width) * (atan2f(n.z, n.x) / (2.0f * pi) + 0.5f));
    int v = (int) (((float) height) * (0.5f - asinf(n.y) / pi));

    png_bytep row = data[v];
    png_bytep pixel = &row[u * 4];

    return glm::vec3((float) pixel[0], (float) pixel[1], (float) pixel[2]) / 255.0f;
}

glm::vec3 Sphere::get_bump_map_pixel(glm::vec3 p) {
    if (bump_map == NULL) {
        // this should not be happened
        assert(false);
    }

    int width = bump_map->width;
    int height = bump_map->height;
    png_bytep *data = bump_map->data;
    
    glm::vec3 n = glm::normalize(p - center);
    float pi = std::atan(1) * 4;
    int u = (int) (((float) width) * (atan2f(n.z, n.x) / (2.0f * pi) + 0.5f));
    int v = (int) (((float) height) * (0.5f - asinf(n.y) / pi));

    png_bytep row = data[v];
    png_bytep pixel = &row[u * 4];

    return glm::vec3((float) pixel[0], (float) pixel[1], (float) pixel[2]) / 255.0f;
}
