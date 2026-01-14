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

ScreenVec to_screen(Vec2 p, ScreenVec image_dim) {
    // -1..1 => 0..2 => 0..1 => 0..w
    return (ScreenVec){
        .x = (p.x+1)/2 * image_dim.x,
        .y = (1 - (p.y+1)/2) * image_dim.y,
    };
}

Vec2 project(Vec3 v) {
    return (Vec2){
        .x = v.x/v.z,
        .y = v.y/v.z,
    };
}

Vec3 vec3_add(Vec3 a, Vec3 b) {
    return (Vec3){
        .x = a.x + b.x,
        .y = a.y + b.y,
        .z = a.z + b.z,
    };
}

Vec3 vec3_sub(Vec3 a, Vec3 b) {
    return (Vec3){
        .x = a.x - b.x,
        .y = a.y - b.y,
        .z = a.z - b.z,
    };
}

Vec3 vec3_rotate(Vec3 v, Vec3 rot) {
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
    if (rot.z) {
        float c = cosf(rot.z);
        float s = sinf(rot.z);
        v = (Vec3){
            .x = v.x*c - v.y*s,
            .y = v.x*s + v.y*c,
            .z = v.z,
        };
    }
    return v;
}

Vec3 vec3_scale(Vec3 v, Vec3 s) {
    return (Vec3){
        .x = v.x * s.x,
        .y = v.y * s.y,
        .z = v.z * s.z,
    };
}

Vec3 vec3_scale_s(Vec3 v, float s) {
    return (Vec3){
        .x = v.x * s,
        .y = v.y * s,
        .z = v.z * s,
    };
}

void vec2_repr(Vec2 v) {
    printf("[%f; %f]\n", v.x, v.y);
}

void vec3_repr(Vec3 v) {
    printf("[%f; %f; %f]\n", v.x, v.y, v.z);
}
