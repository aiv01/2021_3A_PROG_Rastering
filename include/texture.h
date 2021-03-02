#pragma once
#include "SDL_image.h"
#include <string.h>

typedef struct {
    int width;
    int height;
    int pixel_size;
    unsigned char* data;
} texture;

static texture* texture_load(const char* img_path) {
   SDL_Surface* surface = IMG_Load(img_path);
   if (surface == NULL) {
       printf("Error loading image: %s", img_path);
       return NULL;
   }

   texture* t = malloc(sizeof(texture));
   t->width = surface->w;
   t->height = surface->h;
   t->pixel_size = surface->format->BytesPerPixel;
   int data_size = t->width * t->height * t->pixel_size;
   t->data = malloc(data_size);
   memcpy(t->data, surface->pixels, data_size);

   SDL_FreeSurface(surface);
   return t;
}

static void texture_destroy(texture* t) {
    free(t->data);
    free(t);
}