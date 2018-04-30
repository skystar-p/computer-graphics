#ifndef SPLINE_H
#define SPLINE_H

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

// b-spline
template <typename T>
T b_spline(T p0, T p1, T p2, T p3, float t) {
    float b0 =
        (1.0f / 6.0f) * (1.0f - t) * (1.0f - t) * (1.0f - t);
    float b1 =
        (1.0f / 6.0f) * (3.0f * t * t * t - 6.0f * t * t + 4.0f);
    float b2 =
        (1.0f / 6.0f) * (-3.0f * t * t * t + 3.0f * t * t + 3.0f * t + 1.0f);
    float b3 =
        (1.0f / 6.0f) * t * t * t;

    return (p0 * b0) + (p1 * b1) + (p2 * b2) + (p3 * b3);
}

template <>
glm::quat b_spline(glm::quat p0, glm::quat p1, glm::quat p2, glm::quat p3, float t) {
    float b0 =
        (1.0f / 6.0f) * (1.0f - t) * (1.0f - t) * (1.0f - t);
    float b1 =
        (1.0f / 6.0f) * (3.0f * t * t * t - 6.0f * t * t + 4.0f);
    float b2 =
        (1.0f / 6.0f) * (-3.0f * t * t * t + 3.0f * t * t + 3.0f * t + 1.0f);
    float b3 =
        (1.0f / 6.0f) * t * t * t;

    return glm::exp(b0 * glm::log(p0)) * glm::exp(b1 * glm::log(p1)) * glm::exp(b2 * glm::log(p2)) * glm::exp(b3 * glm::log(p3));
}


// bezier curve
template <typename T>
T bezier_curve(T p0, T p1, T p2, T p3, float t) {
    float b0 = (1.0f - t) * (1.0f - t) * (1.0f - t);
    float b1 = 3.0f * t * (1.0f - t) * (1.0f - t);
    float b2 = 3.0f * t * t * (1.0f - t);
    float b3 = t * t * t;

    return (p0 * b0) + (p1 * b1) + (p2 * b2) + (p3 * b3);
}

template <>
glm::quat bezier_curve(glm::quat p0, glm::quat p1, glm::quat p2, glm::quat p3, float t) {
    float b0 = (1.0f - t) * (1.0f - t) * (1.0f - t);
    float b1 = 3.0f * t * (1.0f - t) * (1.0f - t);
    float b2 = 3.0f * t * t * (1.0f - t);
    float b3 = t * t * t;

    return glm::exp(b0 * glm::log(p0)) * glm::exp(b1 * glm::log(p1)) * glm::exp(b2 * glm::log(p2)) * glm::exp(b3 * glm::log(p3));
}

// catmull-rom interpolation
template <typename T>
T catmullrom_spline(T p0, T p3, T tgt0, T tgt3, float t) {
    // using hermite to bezier transform & affine invariance
    T p1 = (tgt0 / 3.0f) + p0;
    T p2 = -(tgt3 / 3.0f) + p3;

    return bezier_curve(p0, p1, p2, p3, t);
}

template<>
glm::quat catmullrom_spline(glm::quat p0, glm::quat p3, glm::quat tgt0, glm::quat tgt3, float t) {
    // using hermite to bezier transform & affine invariance
    glm::quat p1 = p0 * glm::exp(tgt0 / 3.0f);
    glm::quat p2 = p3 * glm::inverse(glm::exp(tgt3 / 3.0f));

    return bezier_curve(p0, p1, p2, p3, t);
}
#endif
