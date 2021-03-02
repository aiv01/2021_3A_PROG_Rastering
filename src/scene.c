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

    s->quad_obj = wobj_from_file("resources/quad.obj");
    s->suzanne_obj = wobj_from_file("resources/suzanne.obj");
    s->suzanne_rotation = 0.f;
    return s;
}

static int math_rand(int min, int max) {
    int range = max - min;
    int r = rand() % range;
    return r + min;
}


static void draw_quad(scene* s) {
    wobj* obj = s->quad_obj;

    for (size_t i = 0; i < obj->triangles_count; i++)
    {
        wobj_triangle* t = &obj->triangles[i];

        vector3 wp1 = vector3_mult( (vector3*)&t->v1.position, 1.f);
        vector3 wp2 = vector3_mult( (vector3*)&t->v2.position, 1.f);
        vector3 wp3 = vector3_mult( (vector3*)&t->v3.position, 1.f);

        vector2 sp1 = camera_world_to_screen_point(s->camera, &wp1);
        vector2 sp2 = camera_world_to_screen_point(s->camera, &wp2);
        vector2 sp3 = camera_world_to_screen_point(s->camera, &wp3);

        color r = {255, 0, 0, 255};
        bound_raster(s->screen, &sp1, &sp2, &sp3, &r);
    }  
}

static void draw_suzanne(scene* s, float delta_time) {
    wobj* obj = s->suzanne_obj;

    boolean fill = true;

    for (size_t i = 0; i < obj->triangles_count; i++)
    {
        wobj_triangle* t = &obj->triangles[i];

        vector3 wp1 = vector3_mult( (vector3*)&t->v1.position, 2.f);
        vector3 wp2 = vector3_mult( (vector3*)&t->v2.position, 2.f);
        vector3 wp3 = vector3_mult( (vector3*)&t->v3.position, 2.f);

        s->suzanne_rotation += 0.01f * delta_time;
        wp1 = vector3_rotate_on_y(&wp1, s->suzanne_rotation);
        wp2 = vector3_rotate_on_y(&wp2, s->suzanne_rotation);
        wp3 = vector3_rotate_on_y(&wp3, s->suzanne_rotation);

        vector2 sp1 = camera_world_to_screen_point(s->camera, &wp1);
        vector2 sp2 = camera_world_to_screen_point(s->camera, &wp2);
        vector2 sp3 = camera_world_to_screen_point(s->camera, &wp3);

        if (fill) {
            int red = math_rand(20, 255);
            int green = math_rand(20, 255);
            int blue = math_rand(20, 255);

            color c = {red, green, blue, 255};
            bound_raster(s->screen, &sp1, &sp2, &sp3, &c);
        } else {
            dda_line(s->screen, &sp1, &sp2);
            dda_line(s->screen, &sp2, &sp3);
            dda_line(s->screen, &sp3, &sp1);
        }
    }  
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

/* Draw Triangle 
    vector2 sp1 = camera_world_to_screen_point(s->camera, &s->triangle.v1);
    vector2 sp2 = camera_world_to_screen_point(s->camera, &s->triangle.v2);
    vector2 sp3 = camera_world_to_screen_point(s->camera, &s->triangle.v3);

    color r = {255, 0, 0, 255};
    bound_raster(s->screen, &sp1, &sp2, &sp3, &r);
*/

    //draw_quad(s);
    draw_suzanne(s, delta_time);

    screen_blit(s->screen);
}

void scene_destroy(scene* s) {
    wobj_destroy(s->suzanne_obj);
    wobj_destroy(s->quad_obj);
    screen_destroy(s->screen);
    camera_destroy(s->camera);
    free(s);
}

