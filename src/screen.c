#include "screen.h"
#include <stdlib.h>
#include <float.h>

screen* screen_new(int w, int h, SDL_Renderer* r) {
    screen* s = (screen*)malloc(sizeof(screen));
    s->width = w;
    s->height = h;
    s->color_format = 4;
    int buffer_size = sizeof(Uint8) * s->width * s->height * s->color_format;
    s->color_buffer = malloc(buffer_size);
    
    //depth buffer allocation
    s->depth_buffer = malloc(sizeof(float) * s->width * s->height);

    s->renderer = r;
    
    s->texture = SDL_CreateTexture(r,   
            SDL_PIXELFORMAT_RGBA32,
            SDL_TEXTUREACCESS_STATIC,
            s->width, s->height);
    return s;
}

void screen_put_pixel(screen* s, int x, int y, float z, color* c) {
    if ( x < 0 || x >= s->width) return;
    if ( y < 0 || y >= s->height) return;

    //SDL_SetRenderDrawColor(s->renderer,c->r,c->g,c->b,c->a);
    //SDL_RenderDrawPoint(s->renderer, x, y);
    int depth_index = y * s->width + x;
    float curr_z = s->depth_buffer[depth_index];

    //Se la z e' maggiore di quella gia presente nel depth buffer la aggiorno
    if(z < curr_z) return;

    s->depth_buffer[depth_index] = z;
    
    int index = (y * s->width + x) * s->color_format;
    s->color_buffer[index + 0] = c->r;
    s->color_buffer[index + 1] = c->g;
    s->color_buffer[index + 2] = c->b;
    s->color_buffer[index + 3] = c->a;
}; 

void screen_blit(screen* s) {
    SDL_UpdateTexture(s->texture, NULL, s->color_buffer, s->width * s->color_format);
    SDL_RenderCopy(s->renderer, s->texture, NULL, NULL);
} 

void screen_clear(screen* s) {
    memset(s->color_buffer, 0, sizeof(Uint8) * s->width * s->height * s->color_format);
    for (int i = 0; i < s->width * s->height; i++)
    {
        s->depth_buffer[i] = -FLT_MAX;
    }
    
}

void screen_destroy(screen* s) {
    SDL_DestroyTexture(s->texture);
    free(s->color_buffer);
    free(s->depth_buffer);
    free(s);
} 