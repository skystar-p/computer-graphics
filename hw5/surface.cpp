#include "object.h"
#include "surface.h"
#include "ray.h"
#include <cmath>

#include <cstdio>

#define EPSILON 1e-2f

Triangle::Triangle(glm::vec3 a, glm::vec3 b, glm::vec3 c,
        glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
        int gloss, float n, bool is_reflective,
        bool is_refractive)
    : Object(ambient, diffuse, specular, gloss, n,
            is_reflective, is_refractive) {

    points.push_back(a);
    points.push_back(b);
    points.push_back(c);

    vn = glm::normalize(glm::cross(a - b, b - c));
}

Ray Triangle::reflect(Ray ray) {
    glm::vec3 intersection = intersect(ray);
    glm::vec3 norm = normal(intersection);
    glm::vec3 l = -ray.direction;

    float n = this->n;
    if (glm::dot(norm, l) < -EPSILON) {
        norm = -norm;
        n = ray.n;
    }

    glm::vec3 r = glm::dot(l * 2.0f, norm) * norm - l;

    return Ray(intersection + r * EPSILON, r, ray.n);
}

Ray Triangle::refract(Ray ray) {
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

bool same_side(glm::vec3 p1, glm::vec3 p2, glm::vec3 a, glm::vec3 b) {
    if (glm::dot(glm::cross(b - a, p1 - a), glm::cross(b - a, p2 - a)) >= 0) {
        return true;
    }
    return false;
}

bool Triangle::includes(glm::vec3 p) {
    glm::vec3 &a = points[0];
    glm::vec3 &b = points[1];
    glm::vec3 &c = points[2];
    if (same_side(p, a, b, c) && same_side(p, b, a, c) && same_side(p, c, a, b)) {
        return true;
    }
    return false;
}

bool Triangle::has_intersection(Ray ray) {
    float ln = glm::dot(ray.direction, vn);
    float d = glm::dot(points[0] - ray.origin, vn) / ln;
    if (d < EPSILON) {
        return false;
    }
    glm::vec3 p = ray.origin + ray.direction * d;
    return includes(p);
}

glm::vec3 Triangle::intersect(Ray ray) {
    float ln = glm::dot(ray.direction, vn);
    float d = glm::dot(points[0] - ray.origin, vn) / ln;
    return ray.origin + ray.direction * d;
}

glm::vec3 Triangle::normal(glm::vec3 p) {
    if (!includes(p)) assert(false);
    return vn;
}

glm::vec3 Triangle::get_texture_pixel(glm::vec3 p) {
    assert(false);
    return p;
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

    float n = this->n;
    if (glm::dot(norm, l) < -EPSILON) {
        norm = -norm;
        n = ray.n;
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

glm::vec3 Surface::get_texture_pixel(glm::vec3 p) {
    // never use this
    assert(false);
    return p;
}
