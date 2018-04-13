#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/freeglut.h>
#include <iostream>
#include <cmath>
#include "camera.h"

Vec::Vec(float x_, float y_, float z_) {
    x = x_; y = y_; z = z_;
}

float Vec::norm() {
    return sqrt(x * x + y * y + z * z);
}

void Vec::normalize() {
    float n = norm();
    x /= n;
    y /= n;
    z /= n;
}

Camera::Camera(Vec c) {
    eye = c;
    lookat();
}

Vec rotate_quater(Vec axis, float angle, Vec rot) {
    float a = cosf(angle / 2);
    float b = sinf(angle / 2) * axis[0];
    float c = sinf(angle / 2) * axis[1];
    float d = sinf(angle / 2) * axis[2];
    float rotated_vector[3];
    rotated_vector[0] = (a * a + b * b - c * c - d * d) * rot[0]
        + (2 * b * c + 2 * a * d) * rot[1]
        + (2 * b * d - 2 * a * c) * rot[2];
    rotated_vector[1] = (2 * b * c - 2 * a * d) * rot[0]
        + (a * a - b * b + c * c - d * d) * rot[1]
        + (2 * c * d + 2 * a * b) * rot[2];
    rotated_vector[2] = (2 * b * d + 2 * a * c) * rot[0]
        + (2 * c * d - 2 * a * b) * rot[1]
        + (a * a - b * b - c * c + d * d) * rot[2];

    return Vec(rotated_vector[0], rotated_vector[1], rotated_vector[2]);
}

void Camera::rotate(Vec start, Vec end) {
    Vec ax = start ^ end;
    ax.normalize();

    float angle = acosf(start * end / (start.norm() * end.norm()));

    eye = rotate_quater(ax, angle, eye);
    up = rotate_quater(ax, angle, up);

    lookat();
}

void Camera::dolly(float dr) {

}

void Camera::translate(Vec t) {
    center = center + t;
    eye = eye + t;
}

Vec Camera::mouse_to_sphere(int x, int y, int width, int height) {

}

void Camera::lookat() {
    gluLookAt(
        eye[0], eye[1], eye[2],
        center[0], center[1], center[2],
        up[0], up[1], up[2]
    );
}
