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

#define vec_repr(v) _Generic(v, \
        Vec2: vec2_repr, \
        Vec3: vec3_repr)(v)

Vec3 vec3_add(Vec3 a, Vec3 b);
Vec3 vec3_rotate(Vec3 v, Vec2 rot);

void vec2_repr(Vec2);
void vec3_repr(Vec3);

#endif
