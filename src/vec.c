#include "vec.h"

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

vector2 vector2_new(int x, int y){
    vector2 v;
    v.x = x;
    v.y = y;
    return v;
}