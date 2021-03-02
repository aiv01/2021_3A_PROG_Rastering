#include "vec.h"
#include "SDL.h"

vector3 vector3_new(float x, float y, float z){
    vector3 v;
    v.x = x;
    v.y = y;
    v.z = z;
    return v;
}

vector3 vector3_sub(vector3* v1, vector3* v2) {
    vector3 r;
    r.x = v1->x - v2->x;
    r.y = v1->y - v2->y;
    r.z = v1->z - v2->z;
    return r;
}

vector3 vector3_mult(vector3* v, float scalar) {
    vector3 r;
    r.x = v->x * scalar;
    r.y = v->y * scalar;
    r.z = v->z * scalar;
    return r;
}

vector3 vector3_rotate_on_y(vector3* v, float degrees) {
    float rads = degrees * M_PI / 180.f;
    vector3 r;
    r.x = cosf(rads) * v->x - sinf(rads) * v->z;
    r.y = v->y;
    r.z = sinf(rads) * v->x + cosf(rads) * v->z;
    return r;
}

vector2 vector2_new(int x, int y){
    vector2 v;
    v.x = x;
    v.y = y;
    return v;
}