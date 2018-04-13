#ifndef CAMERA_H
#define CAMERA_H
#include <stdexcept>

class Vec {
    public:
        float x, y, z;
        Vec(float, float, float);

        Vec operator+ (const Vec& v) {
            Vec new_vec(x + v.x, y + v.y, z + v.z);
            return new_vec;
        }

        Vec operator- (const Vec& v) {
            Vec new_vec(x - v.x, y - v.y, z - v.z);
            return new_vec;
        }

        // dot product
        float operator* (const Vec& v) {
            return x * v.x + y * v.y + z * v.z;
        }

        // cross product
        Vec operator^ (Vec& v) {
            Vec new_vec(
                    y * v.z - z * v.y,
                    z * v.x - x * v.z,
                    x * v.y - y * v.x);
            return new_vec;
        }

        float &operator[] (int i) {
            if (i == 0) return x;
            if (i == 1) return y;
            if (i == 2) return z;
            throw std::invalid_argument("invalid index");
        }
        
        float norm();

        void normalize();
};

class Camera {
    public:
        Camera(Vec);
        void rotate(Vec, Vec);
        void dolly(float);
        void translate(Vec);
        Vec mouse_to_sphere(int x, int y, int width, int height);
        void lookat();

    private:
        Vec center = Vec(0.0, 0.0, 0.0);
        Vec eye = Vec(0.0, 0.0, 30.0);
        Vec up = Vec(0.0, 1.0, 0.0);
};

#endif
