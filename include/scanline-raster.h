#pragma once
#include "vec.h"
#include "screen.h"

typedef struct {
    vector2* screen_pos;
    color* color;
    float z;
} vertex;

// y: v1 < v2 < v3
static void __sort_by_y(vertex** v1, vertex** v2, vertex** v3) {
    vertex* temp;
    if ( (*v1)->screen_pos->y > (*v2)->screen_pos->y) { temp = *v1; *v1 = *v2; *v2 = temp; }
    if ( (*v2)->screen_pos->y > (*v3)->screen_pos->y) { temp = *v2; *v2 = *v3; *v3 = temp; }
    if ( (*v1)->screen_pos->y > (*v2)->screen_pos->y) { temp = *v1; *v1 = *v2; *v2 = temp; }
}

static float __interpolate_scalar(float a, float b, float gradient) {
    return (b - a) * gradient + a;
}

static color __interpolate_color(color* a, color* b, float gradient) {
    color c;
    c.r = (int)__interpolate_scalar(a->r, b->r, gradient);
    c.g = (int)__interpolate_scalar(a->g, b->g, gradient);
    c.b = (int)__interpolate_scalar(a->b, b->b, gradient);
    c.a = (int)__interpolate_scalar(a->a, b->a, gradient);
    return c;
}

static void __interpolate_row(screen* s, int y,
    vertex* left_edge_v1, vertex* left_edge_v2,
    vertex* right_edge_v1, vertex* right_edge_v2
    ) {

    vector2* left_edge_p1 = left_edge_v1->screen_pos;
    vector2* left_edge_p2 = left_edge_v2->screen_pos;
    vector2* right_edge_p1 = right_edge_v1->screen_pos;
    vector2* right_edge_p2 = right_edge_v2->screen_pos;
    
    float left_gradient_y = 1.f;
    if (left_edge_p2->y != left_edge_p1->y)
        left_gradient_y = (float)(y - left_edge_p1->y) / (float)(left_edge_p2->y - left_edge_p1->y);

    float right_gradient_y = 1.f;
    if (right_edge_p2->y != right_edge_p1->y)
        right_gradient_y = (float)(y - right_edge_p1->y) / (float)(right_edge_p2->y - right_edge_p1->y);

    int left_x = (int)__interpolate_scalar(left_edge_p1->x, left_edge_p2->x, left_gradient_y);
    int right_x = (int)__interpolate_scalar(right_edge_p1->x, right_edge_p2->x, right_gradient_y);

    color left_color = __interpolate_color(left_edge_v1->color, left_edge_v2->color, left_gradient_y);
    color right_color = __interpolate_color(right_edge_v1->color, right_edge_v2->color, left_gradient_y);

    float left_z = __interpolate_scalar(left_edge_v1->z, left_edge_v2->z, left_gradient_y);
    float right_z = __interpolate_scalar(right_edge_v1->z, right_edge_v2->z, right_gradient_y);


    for(int x=left_x; x <= right_x; x++) {
        float gradient_x = 1.f;
        if (left_x < right_x) gradient_x = (float)(x - left_x) / (float)(right_x - left_x);
        color c = __interpolate_color(&left_color, &right_color, gradient_x);

        float z = __interpolate_scalar(left_z, right_z, gradient_x);

        screen_put_pixel(s, x, y, z, &c);
    }
}

static void scanline_raster(screen* s, vertex* v1, vertex* v2, vertex* v3) {
    //Individuare il tipo di triangolo
    __sort_by_y(&v1, &v2, &v3);

    float inv_slope_v1v2 = (float)(v2->screen_pos->x - v1->screen_pos->x) / (float)(v2->screen_pos->y - v1->screen_pos->y);
    float inv_slope_v1v3 = (float)(v3->screen_pos->x - v1->screen_pos->x) / (float)(v3->screen_pos->y - v1->screen_pos->y);

    // Triangolo con v2 a sinistra <|
    if (inv_slope_v1v2 < inv_slope_v1v3) {
        for(int y = (int)v1->screen_pos->y; y <= (int)v3->screen_pos->y; y++) {
            if (y < v2->screen_pos->y) { //triangolo superiore
                __interpolate_row(s, y, v1,v2, v1,v3);
            } else { //triangolo inferiore
                __interpolate_row(s, y, v2,v3, v1,v3);
            }
        }
    } else {// Triangolo con v2 a destra |>
        for(int y = (int)v1->screen_pos->y; y <= (int)v3->screen_pos->y; y++) {
            if (y < v2->screen_pos->y) { //triangolo superiore
                __interpolate_row(s, y, v1,v3, v1,v2);
            } else { //triangolo inferiore
                __interpolate_row(s, y, v1,v3, v2,v3);
            }
        }
    }
}