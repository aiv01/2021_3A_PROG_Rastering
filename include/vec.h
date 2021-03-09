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

vector3 vector3_normalize(vector3* v);

float vector3_magnitude(vector3*v);

float vector3_dot(vector3*a ,vector3*b);


typedef struct{
    int x;
    int y;
} vector2;

typedef struct{
    float x;
    float y;
} vector2f;

vector2 vector2_new(int x, int y);
vector2f vector2f_new(float x, float y);
#endif