#pragma once

typedef struct {
    int r;
    int g;
    int b;
    int a;
} color;

static color color_mul_scalar(color *c, float scalar){
    color new_color;
    new_color.r = c->r * scalar;
    new_color.g = c->g * scalar;
    new_color.b = c->b * scalar;
    new_color.a = c->a * scalar;
    return new_color;
}
static color color_sum(color*a, color *b){
    color new_color;
    new_color.r = a->r+b->r;
    new_color.g = a->g+b->g;
    new_color.b = a->b+b->b;
    new_color.a = a->a+b->a;
    return new_color;
}
static int __clamp_color(int val){
    if(val>255){
       return 255;
    }
    if(val<0){
        return 0;
    }
    return val;
}
static color color_clamp(color* c){
    color new_color;
    new_color.r = __clamp_color(c->r);
    new_color.g = __clamp_color(c->g);
    new_color.b = __clamp_color(c->b);
    new_color.a = __clamp_color(c->a);
    return new_color;
}