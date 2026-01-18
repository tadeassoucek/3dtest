#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "vector.h"

#define GR_OBJ_FILE_BUFSIZE 1024

typedef uint8_t byte;
typedef uint32_t colorhex;

typedef struct {
    byte r;
    byte g;
    byte b;
} Color;

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
    // Name of the object.
    char *name;
    // Id number.
    size_t id;
    // Dynamic list of all vertices.
    VertList verts;
    // Dynamic list of all faces.
    FaceList faces;
    // Transform vector. Move the object by this vector in the scene.
    Vec3 transform;
    // Rotation vector.
    Vec3 rotation;
    // Scale vector.
    Vec3 scale;
} Object;

Object load_object_file(FILE *file);

Color to_color(colorhex);

void new_buffer();
void draw_pixel(size_t x, size_t y, Color);
void draw_line(ScreenVec a, ScreenVec b, Color);
void draw_point(ScreenVec p, screenint s, Color);
void draw_object(Object *obj, colorhex vert_color, colorhex line_color);
FILE *write_frame(const char *path);

#endif
