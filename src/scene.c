#include "scene.h"
#include "math.h"
#include "color.h"
#include "line-raster.h"
#include "bound-raster.h"
#include <stdio.h>
#include <string.h>
#include "wobj.h"

scene* scene_create(int screen_width, int screen_height, SDL_Renderer* r) {
    scene* s = malloc(sizeof(scene));
    s->triangle.v1 = vector3_new(0, 2,  -10.f);
    s->triangle.v2 = vector3_new(-2, 0, -10.f);
    s->triangle.v3 = vector3_new(2, 0,  -10.f);
    s->screen = screen_new(screen_width, screen_height, r);

    s->camera = camera_new(60.f, screen_width, screen_height);
    s->camera->position = vector3_new(0, 0, 5);
    return s;
}

static vector3 vector3_mult(vector3* v, float scalar) {
    vector3 r;
    r.x = v->x * scalar;
    r.y = v->y * scalar;
    r.z = v->z * scalar;
    return r;
}
   
void scene_update(scene* s, float delta_time) {
    screen_clear(s->screen);
/*
    vector2 v1 = {0, 0};
    vector2 v2 = {100, 100};
    //dda_line(s->screen, &v1, &v2);

    color r = {255, 0, 0, 255};
    vector2 t1 = { 200, 100 };
    vector2 t2 = { 100, 200 };
    vector2 t3 = { 300, 200 };
    bound_raster(s->screen, &t1, &t2, &t3, &r);
*/  


    vector2 sp1 = camera_world_to_screen_point(s->camera, &s->triangle.v1);
    vector2 sp2 = camera_world_to_screen_point(s->camera, &s->triangle.v2);
    vector2 sp3 = camera_world_to_screen_point(s->camera, &s->triangle.v3);

    color r = {255, 0, 0, 255};
    bound_raster(s->screen, &sp1, &sp2, &sp3, &r);

    screen_blit(s->screen);
}

void scene_destroy(scene* s) {
    screen_destroy(s->screen);
    camera_destroy(s->camera);
    free(s);
}

