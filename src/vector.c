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
    if (rot.x) {
        float c = cosf(rot.x);
        float s = sinf(rot.x);
        v = (Vec3){
            .x = v.x,
            .y = v.y*c - v.z*s,
            .z = v.y*s + v.z*c,
        };
    }
    if (rot.y) {
        float c = cosf(rot.y);
        float s = sinf(rot.y);
        v = (Vec3){
            .x = v.x*c - v.z*s,
            .y = v.y,
            .z = v.x*s + v.z*c,
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
