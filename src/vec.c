#include "vec.h"

vector3 vector3_new(float x, float y, float z){
    vector3 v;
    v.x = x;
    v.y = y;
    v.z = z;
    return v;
}

vector2 vector2_new(int x, int y){
    vector2 v;
    v.x = x;
    v.y = y;
    return v;
}