#include "scene.h"
#include "math.h"
#include "color.h"
#include "line-raster.h"
#include "bound-raster.h"

scene* scene_create(int screen_width, int screen_height, SDL_Renderer* r) {
    scene* s = malloc(sizeof(scene));
    s->triangle.v1 = vector3_new(200, 100, 0);
    s->triangle.v2 = vector3_new(100, 200, 0);
    s->triangle.v3 = vector3_new(300, 200, 0);
    s->screen = screen_new(screen_width, screen_height, r);
    return s;
}
   
void scene_update(scene* s, float delta_time) {
    screen_clear(s->screen);

    vector2 v1 = {0, 0};
    vector2 v2 = {100, 100};
    //dda_line(s->screen, &v1, &v2);

    color r = {255, 0, 0, 255};
    vector2 t1 = { 200, 100 };
    vector2 t2 = { 100, 200 };
    vector2 t3 = { 300, 200 };

    bound_raster(s->screen, &t1, &t2, &t3, &r);

    screen_blit(s->screen);
}

void scene_destroy(scene* s) {
    screen_destroy(s->screen);
    free(s);
}