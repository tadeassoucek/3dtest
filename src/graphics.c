#include <stdio.h>
#include <string.h>
#include "graphics.h"

void pl_alloc(PointList *pl, size_t capacity) {
    pl->points = (Vec3*)malloc(capacity);
    pl->count = 0;
    pl->capacity = capacity;
}

void pl_append(PointList *pl, Vec3 p) {
    if (pl->capacity == 0)
        pl_alloc(pl, 128);
    else if (pl->count + 1 >= pl->capacity)
        pl->points = (Vec3*)realloc(pl->points, pl->capacity *= 2);
    pl->points[pl->count++] = p;
}

Object obj_load(FILE *file) {
    Object obj = {0};
#define BUFSIZE 1024
    char buf[BUFSIZE];
    while (fgets(buf, BUFSIZE, file)) {
        if (buf[0] != 'v' || buf[1] != ' ') continue;
        float x, y, z;
        sscanf(buf, "v %f %f %f", &x, &y, &z);
        printf("creating point at %f %f %f\n", x, y, z);
        pl_append(&obj.points, (Vec3){ x, y, z });
    }
    return obj;
}

ScreenVec image_dim = {0};
byte *image_buffer = NULL;

#define to_buflen(dim) ((size_t)image_dim.x * image_dim.y * 3)

void new_buffer() {
    if (image_buffer == NULL)
        image_buffer = (byte*)malloc(to_buflen(image_dim));
    memset(image_buffer, 0, to_buflen(image_dim));
}

ScreenVec to_screen(Vec2 p) {
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

void inspect_buffer() {
    for (size_t y = 0; y < image_dim.y; y++) {
        for (size_t x = 0; x < image_dim.x; x++) {
            size_t i = image_dim.x * 3 * y + x * 3;
            byte b = image_buffer[i] | image_buffer[i+1] | image_buffer[i+2];
            if (b == 0)
                putchar('.');
            else
                putchar('X');
        }
        putchar('\n');
    }
}

void draw_pixel(size_t x, size_t y) {
    size_t r = image_dim.x * 3 * y;
    size_t c = 3 * x;
    image_buffer[r+c] = 0x00;
    image_buffer[r+c+1] = 0xFF;
    image_buffer[r+c+2] = 0x00;
}

void draw_point(ScreenVec p, screenint s) {
    screenint d = s/2;
    if (p.x > image_dim.x || p.y > image_dim.y)
        return;
    for (int j = -d; j < d; j++)
        for (int i = -d; i < d; i++)
            draw_pixel(p.x + i, p.y + j);
}

void draw_object(Object *obj) {
    PointList *pts = &obj->points;
    for (size_t i = 0; i < pts->count; i++) {
        Vec3 actual = pts->points[i];
        actual = vec3_rotate(actual, obj->rotation);
        actual = vec3_add(actual, obj->transform);
        draw_point(to_screen(project(actual)), 10);
    }
}

FILE *write_frame(const char *path) {
    FILE *file = fopen(path, "wb");
    fprintf(file, "P6\n");
    fprintf(file, "%d %d\n", image_dim.x, image_dim.y);
    fprintf(file, "255\n");
    fwrite(image_buffer, 1, to_buflen(image_dim), file);
    return file;
}
