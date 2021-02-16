#include "screen.h"
#include <stdlib.h>

screen* screen_new(int w, int h, SDL_Renderer* r) {
    screen* s = (screen*)malloc(sizeof(screen));
    s->width = w;
    s->height = h;
    s->color_format = 4;
    int buffer_size = sizeof(Uint8) * s->width * s->height * s->color_format;
    printf("%d\n", buffer_size);
    s->color_buffer = malloc(buffer_size);
    
    s->renderer = r;
    s->texture = SDL_CreateTexture(r,   
            SDL_PIXELFORMAT_RGBA32,
            SDL_TEXTUREACCESS_STATIC,
            s->width, s->height);
    return s;
}

void screen_put_pixel(screen* s, int x, int y, color* c) {
    if ( x < 0 || x >= s->width) return;
    if ( y < 0 || y >= s->height) return;

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
}

void screen_destroy(screen* s) {
    SDL_DestroyTexture(s->texture);
    free(s->color_buffer);
    free(s);
} 