#ifndef SCENE_H
#define SCENE_H
#include <SDL.h>
#include "vec.h"
#include "screen.h"
#include "camera.h"

#include "wobj.h"
#include "texture.h"

typedef struct{
    vector3 v1;
    vector3 v2;
    vector3 v3;
} triangle;

typedef struct {
    triangle triangle;
    screen* screen;
    camera* camera;
    wobj* quad_obj;
    wobj* suzanne_obj;
    float suzanne_rotation;
    wobj* trup_obj;
    texture* trup_texture;
    texture* quad_texture;
} scene;

scene* scene_create(int w, int h, SDL_Renderer*);
void scene_update(scene*, float delta_time);
void scene_destroy(scene*);

#endif //SCENE_H