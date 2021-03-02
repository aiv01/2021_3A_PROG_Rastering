#pragma once
#include "common.h"

static int cmn_min(int a, int b, int c) {
    float r = a;
    if (b < r) r = b;
    if (c < r) r = c;
    return r;
}

static int cmn_max(int a, int b, int c) {
    float r = a;
    if (b > r) r = b;
    if (c > r) r = c;
    return r;
}

static int det(vector2* p, vector2* v1, vector2* v2) {
    //Ax * By - Ay * Bx
    //A = V1V2
    //B = V1P
    int Ax = v2->x - v1->x;
    int Ay = v2->y - v1->y;
    int Bx = p->x - v1->x;  
    int By = p->y - v1->y;
    return Ax * By - Ay * Bx;
}

static boolean is_point_in_triangle(vector2* p, vector2* a, vector2* b, vector2* c) {
    boolean d1 = det(p, a, b) < 0.f;
    boolean d2 = det(p, b, c) < 0.f;
    boolean d3 = det(p, c, a) < 0.f;
    return (d1 == d2 ) && (d2 == d3);
}

static void bound_raster(screen* s, vector2* t0, vector2* t1, vector2* t2, color* c) {
    int x_min = cmn_min(t0->x, t1->x, t2->x);
    int x_max = cmn_max(t0->x, t1->x, t2->x);
    int y_min = cmn_min(t0->y, t1->y, t2->y);
    int y_max = cmn_max(t0->y, t1->y, t2->y);

    for(int x = x_min; x< x_max; x++) {
        for(int y = y_min; y < y_max; y++) {
            vector2 pt = {x, y};
            if (is_point_in_triangle(&pt, t0, t1, t2)) {
                screen_put_pixel(s, x, y, 0, c);
            }
        }
    }
}