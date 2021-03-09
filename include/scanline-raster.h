#pragma once
#include "vec.h"
#include "screen.h"

typedef enum
{
    CONTEXT_FLAG_COLOR,
    CONTEXT_FLAG_TEXTURE
} context_flags;

typedef struct
{
    screen *screen;
    texture *tex;
    context_flags flags;
    color ambient;
    float ambient_intensity;
    vector3 point_light_position;
} context;

typedef struct
{
    vector2f *uv;
    vector2 *screen_pos;
    vector3 *world_normal;
    vector3 *world_pos;
    color *color;
    float z;
} vertex;

// y: v1 < v2 < v3
static void __sort_by_y(vertex **v1, vertex **v2, vertex **v3)
{
    vertex *temp;
    if ((*v1)->screen_pos->y > (*v2)->screen_pos->y)
    {
        temp = *v1;
        *v1 = *v2;
        *v2 = temp;
    }
    if ((*v2)->screen_pos->y > (*v3)->screen_pos->y)
    {
        temp = *v2;
        *v2 = *v3;
        *v3 = temp;
    }
    if ((*v1)->screen_pos->y > (*v2)->screen_pos->y)
    {
        temp = *v1;
        *v1 = *v2;
        *v2 = temp;
    }
}

static float __interpolate_scalar(float a, float b, float gradient)
{
    return (b - a) * gradient + a;
}

static vector2f __interpolate_vector2f(vector2f *a, vector2f *b, float gradient)
{
    vector2f v;
    v.x = __interpolate_scalar(a->x, b->x, gradient);
    v.y = __interpolate_scalar(a->y, b->y, gradient);
    return v;
}

static color __interpolate_color(color *a, color *b, float gradient)
{
    color c;
    c.r = (int)__interpolate_scalar(a->r, b->r, gradient);
    c.g = (int)__interpolate_scalar(a->g, b->g, gradient);
    c.b = (int)__interpolate_scalar(a->b, b->b, gradient);
    c.a = (int)__interpolate_scalar(a->a, b->a, gradient);
    return c;
}

static vector3 __interpolate_vector3(vector3 *a, vector3 *b, float gradient){
    vector3 v;
    v.x = __interpolate_scalar(a->x, b->x, gradient);
    v.y = __interpolate_scalar(a->y, b->y, gradient);
    v.z = __interpolate_scalar(a->z, b->z, gradient);
    return v;
}

static float __clampf(float val, float min, float max){
    if(val>max){
       return max;
    }
    if(val<min){
        return min;
    }
    return val;
}

static void __interpolate_row(context *context, int y,
                              vertex *left_edge_v1, vertex *left_edge_v2,
                              vertex *right_edge_v1, vertex *right_edge_v2)
{

    vector2 *left_edge_p1 = left_edge_v1->screen_pos;
    vector2 *left_edge_p2 = left_edge_v2->screen_pos;
    vector2 *right_edge_p1 = right_edge_v1->screen_pos;
    vector2 *right_edge_p2 = right_edge_v2->screen_pos;

    float left_gradient_y = 1.f;
    if (left_edge_p2->y != left_edge_p1->y)
        left_gradient_y = (float)(y - left_edge_p1->y) / (float)(left_edge_p2->y - left_edge_p1->y);

    float right_gradient_y = 1.f;
    if (right_edge_p2->y != right_edge_p1->y)
        right_gradient_y = (float)(y - right_edge_p1->y) / (float)(right_edge_p2->y - right_edge_p1->y);

    int left_x = (int)__interpolate_scalar(left_edge_p1->x, left_edge_p2->x, left_gradient_y);
    int right_x = (int)__interpolate_scalar(right_edge_p1->x, right_edge_p2->x, right_gradient_y);

    color left_color, right_color;
    if (context->flags == CONTEXT_FLAG_COLOR)
    {
        left_color = __interpolate_color(left_edge_v1->color, left_edge_v2->color, left_gradient_y);
        right_color = __interpolate_color(right_edge_v1->color, right_edge_v2->color, left_gradient_y);
    }

    vector2f left_uv, right_uv;
    if (context->flags == CONTEXT_FLAG_TEXTURE)
    {
        left_uv = __interpolate_vector2f(left_edge_v1->uv, left_edge_v2->uv, left_gradient_y);
        right_uv = __interpolate_vector2f(right_edge_v1->uv, right_edge_v2->uv, right_gradient_y);
    }

    //world normal
    vector3 left_world_normal = __interpolate_vector3(left_edge_v1->world_normal, left_edge_v2->world_normal,left_gradient_y);
    vector3 right_world_normal = __interpolate_vector3(right_edge_v1->world_normal, right_edge_v2->world_normal, right_gradient_y);

    //world pixel position 
    vector3 left_world_position = __interpolate_vector3(left_edge_v1->world_pos, left_edge_v2->world_pos,left_gradient_y);
    vector3 right_world_position = __interpolate_vector3(right_edge_v1->world_pos, right_edge_v2->world_pos, right_gradient_y);


    float left_z = __interpolate_scalar(left_edge_v1->z, left_edge_v2->z, left_gradient_y);
    float right_z = __interpolate_scalar(right_edge_v1->z, right_edge_v2->z, right_gradient_y);

    for (int x = left_x; x <= right_x; x++)
    {
        float gradient_x = 1.f;
        if (left_x < right_x)
            gradient_x = (float)(x - left_x) / (float)(right_x - left_x);

        color c;
        if (context->flags == CONTEXT_FLAG_COLOR)
            c = __interpolate_color(&left_color, &right_color, gradient_x);

        if (context->flags == CONTEXT_FLAG_TEXTURE)
        {
            vector2f uv = __interpolate_vector2f(&left_uv, &right_uv, gradient_x);
            uv.y = 1 - uv.y;
            texture *tex = context->tex;
            int tex_x_coord = uv.x * (float)(tex->width - 1);
            int tex_y_coord = uv.y * (float)(tex->height - 1);
            int tex_index = (tex_y_coord * tex->width + tex_x_coord) * tex->pixel_size;

            //Texture
            c.r = tex->data[tex_index + 0];
            c.g = tex->data[tex_index + 1];
            c.b = tex->data[tex_index + 2];
            c.a = tex->data[tex_index + 3];


        }
        //Ambient
        color c_ambient = color_mul_scalar(&c, context->ambient_intensity );

        //Lambert
        vector3 world_normal = __interpolate_vector3(&left_world_normal, &right_world_normal, gradient_x);
        world_normal = vector3_normalize(&world_normal);
        vector3 world_position = __interpolate_vector3(&left_world_position, &right_world_position, gradient_x);
        vector3 dir_to_light = vector3_sub(&context->point_light_position,&world_position);
        dir_to_light = vector3_normalize(&dir_to_light);
        float lambert_value = vector3_dot(&dir_to_light,&world_normal) ;
        lambert_value = __clampf(lambert_value, 0.f,1.f);
        color c_lambert = color_mul_scalar(&c, lambert_value);

        


        color final_color = c_ambient;
        final_color = color_sum(&final_color, &c_lambert);
        final_color = color_clamp(&final_color);
        float z = __interpolate_scalar(left_z, right_z, gradient_x);
        screen_put_pixel(context->screen, x, y, z, &final_color);
    }
}

static void scanline_raster(context *c, vertex *v1, vertex *v2, vertex *v3)
{
    //Individuare il tipo di triangolo
    __sort_by_y(&v1, &v2, &v3);

    float inv_slope_v1v2 = (float)(v2->screen_pos->x - v1->screen_pos->x) / (float)(v2->screen_pos->y - v1->screen_pos->y);
    float inv_slope_v1v3 = (float)(v3->screen_pos->x - v1->screen_pos->x) / (float)(v3->screen_pos->y - v1->screen_pos->y);

    // Triangolo con v2 a sinistra <|
    if (inv_slope_v1v2 < inv_slope_v1v3)
    {
        for (int y = (int)v1->screen_pos->y; y <= (int)v3->screen_pos->y; y++)
        {
            if (y < v2->screen_pos->y)
            { //triangolo superiore
                __interpolate_row(c, y, v1, v2, v1, v3);
            }
            else
            { //triangolo inferiore
                __interpolate_row(c, y, v2, v3, v1, v3);
            }
        }
    }
    else
    { // Triangolo con v2 a destra |>
        for (int y = (int)v1->screen_pos->y; y <= (int)v3->screen_pos->y; y++)
        {
            if (y < v2->screen_pos->y)
            { //triangolo superiore
                __interpolate_row(c, y, v1, v3, v1, v2);
            }
            else
            { //triangolo inferiore
                __interpolate_row(c, y, v1, v3, v2, v3);
            }
        }
    }
}