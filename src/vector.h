#ifndef VECTOR_H
#define VECTOR_H

#include <stdint.h>

typedef uint16_t screenint;

typedef struct {
    screenint x, y;
} ScreenVec;

typedef struct {
    float x, y, z;
} Vec2;

typedef struct {
    float x, y, z;
} Vec3;

ScreenVec screen_vec(screenint x, screenint y);
Vec3 vec3(float x, float y, float z);

// Converts from a [-1..1] coord system to a [0..WIDTH|HEIGHT] system
ScreenVec to_screen(Vec2 p, ScreenVec image_dim);
// Projects a 3D vector on a 2D screen.
Vec2 project(Vec3 v);

#define vec_repr(v) _Generic(v, \
        Vec2: vec2_repr, \
        Vec3: vec3_repr)(v)

Vec3 vec3_add(Vec3 a, Vec3 b);
Vec3 vec3_sub(Vec3 a, Vec3 b);
Vec3 vec3_rotate(Vec3 v, Vec3 rot);
Vec3 vec3_scale(Vec3 v, Vec3 s);
Vec3 vec3_scale_s(Vec3 v, float s);

void vec2_repr(Vec2);
void vec3_repr(Vec3);

#endif
