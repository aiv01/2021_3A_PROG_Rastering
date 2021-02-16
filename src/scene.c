#include "scene.h"
#include "math.h"
#include "color.h"
scene* scene_create(int screen_width, int screen_height, SDL_Renderer* r) {
    scene* s = malloc(sizeof(scene));
    s->triangle.v1 = vector3_new(200, 100, 0);
    s->triangle.v2 = vector3_new(100, 200, 0);
    s->triangle.v3 = vector3_new(300, 200, 0);
    s->screen = screen_new(screen_width, screen_height, r);
    return s;
}

void dda_line(screen* scr, vector2* v1, vector2* v2) {
    int delta_x = abs(v1->x - v2->x);
    int delta_y = abs(v1->y - v2->y);
    
    int sign_x = (v1->x > v2->x) ? -1 : +1;
    int sign_y = (v1->y > v2->y) ? -1 : +1;

    int steps;
    if(delta_x > delta_y){
        steps = delta_x;
    }
    else{
        steps = delta_y;
    }
    
    float step_x = (float)delta_x * sign_x / (float)steps;
    float step_y = (float)delta_y * sign_y / (float)steps;
    
    float x = v1->x;
    float y = v1->y;
    
    color red = {255, 0, 0, 0};
    for (int i = 0; i < steps; i++)
    {   
        //SDL_SetRenderDrawColor(scr->renderer,255,0,0,255);
        //SDL_RenderDrawPoint(scr->renderer,(int)x,(int)y);

        screen_put_pixel(scr, (int)x, (int)y, &red);
        x += step_x;
        y += step_y;
    }
}   
void scene_update(scene* s, float delta_time) {
    screen_clear(s->screen);

    vector2 v1 = {0, 0};
    vector2 v2 = {100, 100};

    dda_line(s->screen, &v1, &v2);

    screen_blit(s->screen);
}

void scene_destroy(scene* s) {
    screen_destroy(s->screen);
    free(s);
}