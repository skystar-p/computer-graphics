#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/freeglut.h>
#include <iostream>
#include <cmath>
#include "camera.h"

extern int width, height;

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

    eye = rotate_quater(ax, angle, prev_eye);
    up = rotate_quater(ax, angle, prev_up);
    right = rotate_quater(ax, angle, prev_right);

    lookat();
}

void Camera::dolly(float dr) {
    float current_dist = eye.norm();
    float next_dist = dr + current_dist;
    float ratio = next_dist / current_dist;

    eye = eye * ratio;
}

void Camera::zoom(float dfov) {
    fov += dfov;

    if (fov < 0.1f) {
        fov = 0.1f;
    }
    else if (fov > 60.0f) {
        fov = 60.0f;
    }

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fov, (GLfloat)width / (GLfloat)height, .1f, 500.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    lookat();
}

void Camera::translate(Vec t) {
    Vec tt = (right * t.x - up * t.y) * 0.1f;
    center = prev_center + tt;
    eye = prev_eye + tt;
    lookat();
}

Vec Camera::to_sphere(int x, int y, int width, int height) {
    float nx = (float)x - (float)width / 2.0f;
    float ny = (float)height / 2.0f - (float)y;
    
    Vec p = prev_eye + (prev_up * ny) + (prev_right * nx);

    float r = 300.0f;

    float t = (p * prev_eye - sqrt((p * prev_eye) * (p * prev_eye)
                - (prev_eye * prev_eye) * ((p * p) - r * r))) / (prev_eye * prev_eye);

    if (t != t) {
        t = (p * prev_eye) / (prev_eye * prev_eye);
    }

    return p - prev_eye * t;
}

void Camera::update_vectors() {
    prev_eye = eye;
    prev_up = up;
    prev_right = right;
    prev_center = center;
}

void Camera::lookat() {
    gluLookAt(
        eye[0], eye[1], eye[2],
        center[0], center[1], center[2],
        up[0], up[1], up[2]
    );
}

void Camera::show_all() {
    float r = 50.0f;
    float theta = fov;

    float d = abs(r / sin(theta / 2));

    center = Vec(0.0f, 0.0f, 0.0f);
    eye = eye * (d / eye.norm());
}
