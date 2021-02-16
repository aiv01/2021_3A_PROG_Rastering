#ifndef VECTOR3_H
#define VECTOR3_H


typedef struct{
    float x;
    float y;
    float z;
} vector3;

vector3 vector3_new(float x, float y, float z);

typedef struct{
    int x;
    int y;
} vector2;

vector2 vector2_new(int x, int y);
#endif