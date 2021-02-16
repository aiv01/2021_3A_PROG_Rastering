#pragma once
#include "SDL.h"
#include "color.h"

typedef struct {
    int width;
    int height;
    Uint8* color_buffer;
    int color_format;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
} screen;


screen* screen_new(int w, int h, SDL_Renderer* r); 

void screen_put_pixel(screen* s, int x, int y, color* c); //color;

void screen_blit(screen* s); 
void screen_clear(screen* s);

void screen_destroy(screen* s); 