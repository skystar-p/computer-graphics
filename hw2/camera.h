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

        
        Vec operator* (const float c) {
            return Vec(x * c, y * c, z * c);
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

inline Vec operator* (const float c, const Vec &v) {
    return Vec(v.x * c, v.y * c, v.z * c);
}


class Camera {
    public:
        Camera(Vec);
        void rotate(Vec, Vec);
        void dolly(float);
        void zoom(float);
        void translate(Vec);
        Vec to_sphere(int x, int y, int width, int height);
        void lookat();
        void update_vectors();
        void show_all();

    private:
        Vec center = Vec(0.0, 0.0, 0.0);
        Vec eye = Vec(0.0, 0.0, 30.0);
        Vec up = Vec(0.0, 1.0, 0.0);
        Vec right = Vec(1.0, 0.0, 0.0);

        Vec prev_center = Vec(0.0, 0.0, 0.0);
        Vec prev_eye = Vec(0.0, 0.0, 30.0);
        Vec prev_up = Vec(0.0, 1.0, 0.0);
        Vec prev_right = Vec(1.0, 0.0, 0.0);

        float fov = 45.0f;
};

#endif
