#include "surface.h"
#include "object.h"
#include "ray.h"
#include <cmath>

#define EPSILON 1e-5f

Triangle::Triangle(glm::vec3 a, glm::vec3 b, glm::vec3 c,
        glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
        int gloss, float n, bool is_reflective,
        bool is_refractive)
    : Object(ambient, diffuse, specular, gloss, n,
            is_reflective, is_refractive) {

    points.push_back(a);
    points.push_back(b);
    points.push_back(c);

    // is this right face?
    vn = glm::normalize(glm::cross(a - b, b - c));
}

bool Triangle::includes(glm::vec3 p) {
    // use barycentric coordinate
    float area2 = glm::length(glm::cross(points[1] - points[0],
                points[2] - points[0]));

    float a = glm::length(glm::cross(points[1] - p, points[2] - p)) / area2;
    float b = glm::length(glm::cross(points[2] - p, points[0] - p)) / area2;
    float c = 1 - a - b;

    if (a >= 0 && a <= 1 && b >= 0 && b <= 1 && c >= 0 && c <= 1) {
        return true;
    }
    return false;
}

bool Triangle::has_intersection(Ray ray) {
    float ln = glm::dot(ray.direction, vn);
    if (ln < EPSILON) {
        return false;
    }
    return true;
}

glm::vec3 Triangle::intersect(Ray ray) {
    float ln = glm::dot(ray.direction, vn);
    float d = glm::dot(points[0] - ray.origin, vn) / ln;
    return ray.origin + ray.direction * d;
}

bool Surface::has_intersection(Ray ray) {
    for (unsigned i = 0; i < triangles.size(); i++) {
        if (triangles[i].has_intersection(ray)) {
            return true;
        }
    }
    return false;
}

glm::vec3 Surface::intersect(Ray ray) {
    bool first = false;
    glm::vec3 min_intersect, intersection;

    for (unsigned i = 0; i < triangles.size(); i++) {
        if (triangles[i].has_intersection(ray)) {
            intersection = triangles[i].intersect(ray);
            float d = glm::length(ray.origin - intersection);
            if (!first || glm::length(ray.origin - min_intersect) > d) {
                min_intersect = intersection;
                first = true;
            }
        }
    }

    return intersection;
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
    for (unsigned i = 0; i < triangles.size(); i++) {
        if (triangles[i].includes(point)) {
            return triangles[i].vn;
        }
    }

    // this should not be happen
    assert(false);
    return glm::vec3(0);
}
