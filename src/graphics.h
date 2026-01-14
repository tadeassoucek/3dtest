#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "vector.h"

#define GR_OBJ_FILE_BUFSIZE 1024

typedef struct {
    size_t *verts;
    size_t vertc;
} Face;

typedef struct {
    Vec3 *items;
    size_t capacity;
    size_t count;
} VertList;

typedef struct {
    Face *items;
    size_t capacity;
    size_t count;
} FaceList;

#define dl_append(list, el) do { \
        if (list.count + 1 >= list.capacity) { \
            if (list.capacity == 0) list.capacity = 128; \
            else list.capacity *= 2; \
            list.items = realloc(list.items, list.capacity * sizeof(*list.items)); \
        } \
        list.items[list.count++] = el; \
    } while(0);

typedef struct {
    VertList verts;
    FaceList faces;
    Vec3 transform;
    Vec3 rotation;
    Vec3 scale;
} Object;

Object load_object_file(FILE *file);

typedef uint8_t byte;

void new_buffer();
void draw_pixel(size_t x, size_t y, int color);
void draw_line(ScreenVec a, ScreenVec b, int color);
void draw_point(ScreenVec p, screenint s, int color);
void draw_object(Object *obj, int vert_color, int line_color);
FILE *write_frame(const char *path);

#endif
