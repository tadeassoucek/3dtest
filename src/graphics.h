#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "vector.h"

typedef struct {
    Vec3 *points;
    size_t capacity;
    size_t count;
} PointList;

void pl_alloc(PointList *pl, size_t capacity);
void pl_append(PointList *pl, Vec3 p);

typedef struct {
    PointList points;
    Vec3 transform;
    Vec2 rotation;
} Object;

Object obj_load(FILE *file);

typedef uint8_t byte;

void new_buffer();
void inspect_buffer();
void draw_pixel(size_t x, size_t y);
void draw_point(ScreenVec p, screenint s);
void draw_object(Object *obj);
FILE *write_frame(const char *path);

#endif
