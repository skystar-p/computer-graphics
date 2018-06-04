#include "sphere.h"
#include "object.h"
#include "ray.h"
#include <cmath>

#include <cstdio>

#define EPSILON 1e-5f


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
    if (s < EPSILON) {
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

    if (glm::dot(norm, l) < -EPSILON) {
        norm = -norm;
    }

    glm::vec3 r = 2 * glm::dot(l, norm) * norm - l;

    return Ray(intersection, r, ray.n);
}

Ray Sphere::refract(Ray ray) {
    glm::vec3 intersection = intersect(ray);
    glm::vec3 norm = normal(intersection);
    glm::vec3 l = -ray.direction;

    float n = this->n;

    if (glm::dot(norm, l) < -EPSILON) {
        norm = -norm;
        n = ray.n;
    }

    float cos_i = glm::dot(l, norm);
    float cos_r =
        sqrt(1.0f - (ray.n / n) * (ray.n / n) * (1.0f - cos_i * cos_i));
    glm::vec3 t = ((ray.n / n) * cos_i - cos_r) * norm - (ray.n / n) * l;

    return Ray(intersection, glm::normalize(t), n);
}

glm::vec3 Sphere::normal(glm::vec3 point) {
    return glm::normalize(point - center);
}
