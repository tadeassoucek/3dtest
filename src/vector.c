#include <stdio.h>
#include <math.h>
#include "vector.h"

ScreenVec screen_vec(screenint x, screenint y) {
    return (ScreenVec){
        .x = x,
        .y = y
    };
}

Vec3 vec3(float x, float y, float z) {
    return (Vec3){
        .x = x,
        .y = y,
        .z = z,
    };
}

Vec3 vec3_add(Vec3 a, Vec3 b) {
    return (Vec3){
        .x = a.x + b.x,
        .y = a.y + b.y,
        .z = a.z + b.z,
    };
}

Vec3 vec3_rotate(Vec3 v, Vec2 rot) {
    float x = v.x, y = v.y, z = v.z;
    if (rot.x) {
        float c = cosf(rot.x);
        float s = sinf(rot.x);
        v = (Vec3){
            .x = x,
            .y = y*c - z*s,
            .z = y*s + z*c,
        };
    }
    if (rot.y) {
        float c = cosf(rot.y);
        float s = sinf(rot.y);
        v = (Vec3){
            .x = x*c - z*s,
            .y = y,
            .z = x*s + z*c,
        };
    }
    return v;
}

void vec2_repr(Vec2 v) {
    printf("[%f; %f]\n", v.x, v.y);
}

void vec3_repr(Vec3 v) {
    printf("[%f; %f; %f]\n", v.x, v.y, v.z);
}
