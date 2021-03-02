#ifndef VECTOR3_H
#define VECTOR3_H


typedef struct{
    float x;
    float y;
    float z;
} vector3;

vector3 vector3_new(float x, float y, float z);

vector3 vector3_sub(vector3* v1, vector3* v2);

vector3 vector3_mult(vector3* v, float scalar);

vector3 vector3_rotate_on_y(vector3* v, float degrees);


typedef struct{
    int x;
    int y;
} vector2;

vector2 vector2_new(int x, int y);
#endif