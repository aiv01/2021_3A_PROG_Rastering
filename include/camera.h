#pragma once
#include "vec.h"
#include <stdlib.h>
#include <SDL.h>

typedef struct {
    vector3 position;
    float fov_y_degrees;
    int width;
    int height;
    float aspect_ratio;
    float fov_tan;
} camera;

static camera* camera_new(float fov_degrees, int width, int height) {
    camera* c = (camera*)malloc(sizeof(camera));
    c->position = vector3_new(0, 0, 0);
    c->fov_y_degrees = fov_degrees;
    c->width = width;
    c->height = height;
    c->aspect_ratio = (float)width / (float)height;
    float half_fov = c->fov_y_degrees / 2.f;
    float half_fov_rads = half_fov * M_PI / 180.f;
    c->fov_tan = tanf(half_fov_rads);
    return c;
}

static void camera_destroy(camera* cam) {
    free(cam);
}

static vector2 camera_world_to_screen_point(camera* cam, vector3* world_point) {
    //WORLD -> CAMERA Space
    vector3 camera_point = vector3_sub(world_point, &cam->position);

    //CAMERA -> NDC Space {-1, 1}
    /// Perspective
    float plane_x = camera_point.x / -camera_point.z;
    float plane_y = camera_point.y / -camera_point.z;
    ///Aspect ratio and Fov
    plane_x /= (cam->fov_tan * cam->aspect_ratio);
    plane_y /= cam->fov_tan;

    //NDC -> Screen Space {width, height}
    int screen_x = (int)((plane_x + 1.f)*0.5f * cam->width);
    int screen_y = (int)( (1.f - (plane_y + 1.f)*0.5f ) * cam->height );
    return vector2_new(screen_x, screen_y);
}

static vector3 camera_world_to_camera_point(camera* cam, vector3* world_point){
    return vector3_sub(world_point, &cam->position);
}