#include "surface.h"
#include "object.h"
#include "ray.h"
#include <cmath>

#define EPSILON 1e-5f


Quad::Quad(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d) {
    points.push_back(a);
    points.push_back(b);
    points.push_back(c);
    points.push_back(d);

    vn = glm::normalize(glm::cross(a - c, b - d));
}

bool Quad::includes(glm::vec3 p) {
    glm::vec3 sa = points[1] - points[0], sb = points[2] - points[1];
    glm::vec3 sc = points[3] - points[2], sd = points[0] - points[3];
    glm::vec3 ea = p - sa, eb = p - sb, ec = p - sc, ed = p - sd;
    float fa = glm::cross(sa, ea);

    // TODO
}

bool Surface::has_intersection(Ray ray) {
    glm::vec3 delta = center - ray.origin;
    float t = glm::dot(delta, ray.direction);
    float d = t * t - (glm::length(delta) - radius * radius);
    if (d < EPSILON) {
        return false;
    }
    return true;
}

glm::vec3 Surface::intersect(Ray ray) {
    glm::vec3 delta = center - ray.origin;
    float t = glm::dot(delta, ray.direction);
    float d = t * t - (glm::length(delta) - radius * radius);
    // use Sphere::has_intersection to verify validity of this section
    float s = t - sqrtf(d);

    // check if s is smaller than EPSILON?
    return ray.origin + s * ray.direction;
}

Ray Surface::reflect(Ray ray) {
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

Ray Surface::refract(Ray ray) {
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

glm::vec3 Surface::normal(glm::vec3 point) {
    return vn;
}
