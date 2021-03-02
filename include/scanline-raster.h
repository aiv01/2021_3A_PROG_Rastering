#pragma once
#include "vec.h"
#include "screen.h"


// y: v1 < v2 < v3
static void __sort_by_y(vector2** v1, vector2** v2, vector2** v3) {
    vector2* temp;
    if ( (*v1)->y > (*v2)->y) { temp = *v1; *v1 = *v2; *v2 = temp; }
    if ( (*v2)->y > (*v3)->y) { temp = *v2; *v2 = *v3; *v3 = temp; }
    if ( (*v1)->y > (*v2)->y) { temp = *v1; *v1 = *v2; *v2 = temp; }
}

static float __interpolate_scalar(float a, float b, float gradient) {
    return (b - a) * gradient + a;
}

static void __interpolate_row(screen* s, int y,
    vector2* left_edge_p1, vector2* left_edge_p2,
    vector2* right_edge_p1, vector2* right_edge_p2
    ) {
    
    float left_gradient_y = 1.f;
    if (left_edge_p2->y != left_edge_p1->y)
        left_gradient_y = (float)(y - left_edge_p1->y) / (float)(left_edge_p2->y - left_edge_p1->y);

    float right_gradient_y = 1.f;
    if (right_edge_p2->y != right_edge_p1->y)
        right_gradient_y = (float)(y - right_edge_p1->y) / (float)(right_edge_p2->y - right_edge_p1->y);

    int left_x = (int)__interpolate_scalar(left_edge_p1->x, left_edge_p2->x, left_gradient_y);
    int right_x = (int)__interpolate_scalar(right_edge_p1->x, right_edge_p2->x, right_gradient_y);

    color r = {255, 0, 0, 255};
    for(int x=left_x; x <= right_x; x++) {
        screen_put_pixel(s, x, y, &r);
    }
}

static void scanline_raster(screen* s, vector2* v1, vector2* v2, vector2* v3) {
    //Individuare il tipo di triangolo
    __sort_by_y(&v1, &v2, &v3);

    float inv_slope_v1v2 = (float)(v2->x - v1->x) / (float)(v2->y - v1->y);
    float inv_slope_v1v3 = (float)(v3->x - v1->x) / (float)(v3->y - v1->y);

    // Triangolo con v2 a sinistra <|
    if (inv_slope_v1v2 < inv_slope_v1v3) {
        for(int y = (int)v1->y; y <= (int)v3->y; y++) {
            if (y < v2->y) { //triangolo superiore
                __interpolate_row(s, y, v1,v2, v1,v3);
            } else { //triangolo inferiore
                __interpolate_row(s, y, v2,v3, v1,v3);
            }
        }
    } else {// Triangolo con v2 a destra |>
        for(int y = (int)v1->y; y <= (int)v3->y; y++) {
            if (y < v2->y) { //triangolo superiore
                __interpolate_row(s, y, v1,v3, v1,v2);
            } else { //triangolo inferiore
                __interpolate_row(s, y, v1,v3, v2,v3);
            }
        }
    }
}