#include <glm/glm.hpp>
#include <cmath>
#include <vector>
#include <thread>
#include "world.h"
#include "sphere.h"
#include "object.h"

#include <cstdio>

#define EPSILON 1e-2f
#define DEPTH_MAX 20
#define THREAD_COUNT 16

extern glm::vec3 background;

void *thread_func(void *);


glm::vec3 World::trace(Ray ray, int depth) {
    if (depth > DEPTH_MAX) {
        return background;
    }

    glm::vec3 color;
    float total_coeff = 0.0f;
    glm::vec3 cv = glm::vec3(INFINITY);
    Object *intersect_object = NULL;

    // find first intersection of ray and world's objects
    for (unsigned i = 0; i < objects.size(); i++) {
        if (objects[i]->has_intersection(ray)) {
            glm::vec3 inter = objects[i]->intersect(ray);
            float l = glm::distance(ray.origin, inter);
            if (glm::distance(ray.origin, cv) > l) {
                cv = inter;
                intersect_object = objects[i];
            }
        }
    }

    if (intersect_object == NULL) {
        return background;
    }

    color += get_color(intersect_object, cv);
    total_coeff += 1.0f;

    if (intersect_object->is_reflective) {
        float k = intersect_object->reflect_coeff;
        Ray reflect_ray = intersect_object->reflect(ray);
        color += trace(reflect_ray, depth + 1) * k;
        total_coeff += intersect_object->reflect_coeff;
    }

    if (intersect_object->is_refractive) {
        float k = intersect_object->refract_coeff;
        Ray refract_ray = intersect_object->refract(ray);
        color += trace(refract_ray, depth + 1) * k;
        total_coeff += intersect_object->refract_coeff;
    }

    return color / total_coeff;
}

glm::vec3 World::get_color(Object *object, glm::vec3 intersection) {
    glm::vec3 color = object->ambient;

    // sphere only
    if (object->texture != NULL) {
        color = object->get_texture_pixel(intersection);
    }

    for (unsigned i = 0; i < lights.size(); i++) {
        if (is_reachable(lights[i], intersection)) {
            glm::vec3 n = object->normal(intersection);
            glm::vec3 l = -glm::normalize(intersection - lights[i].position);
            glm::vec3 r = glm::dot(2.0f * l, n) * n - l;
            glm::vec3 v = glm::normalize(eye - intersection);
            float nl = glm::dot(n, l);
            float rv = glm::dot(r, v);
            // needs for attenuation
            float d = glm::distance(intersection, lights[i].position);

            if (nl > EPSILON) {
                glm::vec3 diffuse_color = object->diffuse * lights[i].intensity * glm::dot(n, l) / (d * d);
                color += diffuse_color;
            }

            if (rv > EPSILON) {
                glm::vec3 specular_color = object->specular * lights[i].intensity * powf(rv, object->gloss) / (d * d);
                color += specular_color;
            }
        }
    }

    return color;
}

bool World::is_reachable(Light light, glm::vec3 point) {
    Ray light_ray = Ray(light.position,
            glm::normalize(point - light.position), 0.0f);
    float d = glm::distance(point, light.position);

    for (unsigned i = 0; i < objects.size(); i++) {
        if (objects[i]->has_intersection(light_ray)) {
            float dist_from_light = glm::distance(light.position,
                    objects[i]->intersect(light_ray));
            if (dist_from_light - d < -EPSILON) {
                return false;
            }
        }
    }

    return true;
}

void World::add_light(Light & light) {
    lights.push_back(light);
}

std::vector<glm::vec3> World::render(
        float width, float height, int out_width, int out_height) {
    pthread_t threads[THREAD_COUNT];
    thread_data_t thread_data[THREAD_COUNT];
    int step = out_height / THREAD_COUNT;

    for (int i = 0; i < THREAD_COUNT; i++) {
        // fill thread data
        thread_data[i].id = i;
        thread_data[i].start = step * i;
        thread_data[i].end = i == THREAD_COUNT - 1 ? out_height : step * (i + 1);
        thread_data[i].eye = eye;
        thread_data[i].width = width;
        thread_data[i].height = height;
        thread_data[i].out_width = out_width;
        thread_data[i].out_height = out_height;
        thread_data[i].world = this;

        printf("Spawning thread %d...\n", i);
        pthread_create(&threads[i], NULL, thread_func, (void *)&thread_data[i]);
    }

    std::vector<glm::vec3> data;
    data.clear();

    for (int i = 0; i < THREAD_COUNT; i++) {
        std::vector<glm::vec3> *tdata;
        pthread_join(threads[i], (void **)&tdata);
        data.insert(data.end(), tdata->begin(), tdata->end());
    }

    return data;
}

void *thread_func(void *thread_data) {
    thread_data_t *td = (thread_data_t *) thread_data;
    int id = td->id;
    int start = td->start, end = td->end;
    int width = td->width;
    int height = td->height;
    int out_width = td->out_width;
    int out_height = td->out_height;
    glm::vec3 eye = td->eye;
    World *world = td->world;

    const glm::vec3 o(-width / 2.f, height / 2.f, 0.0f);

    float width_r = width / (float) out_width;
    float height_r = height / (float) out_height;

    const glm::vec3 w(width_r, 0.0f, 0.0f);
    const glm::vec3 h(0.0f, -height_r, 0.0f);

    std::vector<glm::vec3> *data = (std::vector<glm::vec3> *) malloc(sizeof(std::vector<glm::vec3>));
    int progress = -1, prev_progress = 0;

    for (int j = start; j < end; j++) {
        for (int i = 0; i < out_width; i++) {
            prev_progress = ((j - start) * out_width + i) * 100 / (out_width * (end - start) - 1);
            if (progress != prev_progress) {
                progress = prev_progress;
                if (progress % 10 == 0) {
                    printf("Thread %d - %d/100 completed...\n", id, progress);
                }
            }
            glm::vec3 summed(0.0f);
            for (int dj = -2; dj <= 2; dj++) {
                for (int di = -2; di <= 2; di++) {
                    glm::vec3 project = glm::normalize(
                            o + ((float) (i + di * 0.5f) * w) +
                            ((float) (j + dj * 0.5f) * h) - eye);

                    Ray ray(eye, project, 1.0f);
                    glm::vec3 traced_result = world->trace(ray, 0);
                    summed += traced_result;
                }
            }
            data->push_back(summed / 25.0f);
        }
    }

    return (void *) data;
}
