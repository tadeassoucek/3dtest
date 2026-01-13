#include <stdio.h>
#include <string.h>
#include <math.h>
#include "graphics.h"

#define FOREGROUND_COLOR 0x00FF00

Object load_object_file(FILE *file) {
    Object obj = {0};
    char name[GR_OBJ_FILE_BUFSIZE];
    char buf[GR_OBJ_FILE_BUFSIZE];
    while (fgets(buf, GR_OBJ_FILE_BUFSIZE, file)) {
        if (buf[0] == 'o') {
            sscanf(buf, "o %s", name);
        }
        else if (buf[0] == 'v' && buf[1] == ' ') {
            float x, y, z;
            sscanf(buf, "v %f %f %f", &x, &y, &z);
            //printf("creating point at %f %f %f\n", x, y, z);
            dl_append(obj.verts, ((Vec3){ x, y, z }));
        }
        else if (buf[0] == 'f') {
            int verts[4];
            int read = sscanf(buf, "f %d/%*d/%*d %d/%*d/%*d %d/%*d/%*d %d/%*d/%*d", &verts[0], &verts[1], &verts[2], &verts[3]);
            Face face = {
                .verts = malloc(read * sizeof(Face)),
                .vertc = read
            };
            for (size_t i = 0; i < read; i++) {
                face.verts[i] = verts[i];
            }
            dl_append(obj.faces, face);
        }
    }
    printf("> loaded object \"%s\" with %zu vertices and %zu faces\n", name, obj.verts.count, obj.faces.count);
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

void draw_pixel(size_t x, size_t y, int color) {
    if (x >= image_dim.x || y >= image_dim.y) return;
    size_t r = image_dim.x * 3 * y;
    size_t c = 3 * x;
    image_buffer[r+c] = color & 0xFF;
    image_buffer[r+c+1] = (color & 0xFF00) >> 8;
    image_buffer[r+c+2] = (color & 0xFF0000) >> 16;
}

// this function is very silly and doesn't implement any anti-aliasing
// oh well.
void draw_line(ScreenVec a, ScreenVec b, int color) {
    // we presume a is to the left of b; if not, switch them
    if (a.x > b.x) {
        ScreenVec t = a;
        a = b;
        b = t;
    }
    
    int dx = b.x - a.x;
    int dy = b.y - a.y;
    float m = (float)dy/dx;
    float y = a.y;
    for (int x = a.x; x < b.x; x++) {
        draw_pixel(x, (screenint)y, color);
        y += m;
    }
}

// draws a rectangle around the specified point
void draw_point(ScreenVec p, screenint s, int color) {
    screenint d = s/2;
    if (p.x > image_dim.x || p.y > image_dim.y)
        return;
    for (int j = -d; j < d; j++)
        for (int i = -d; i < d; i++)
            draw_pixel(p.x + i, p.y + j, color);
}

void draw_object(Object *obj) {
    VertList *pts = &obj->verts;
    ScreenVec *projected = malloc(pts->count * sizeof(ScreenVec));
    for (size_t i = 0; i < pts->count; i++) {
        Vec3 actual = pts->items[i];
        actual = vec3_rotate(actual, obj->rotation);
        actual = vec3_add(actual, obj->transform);
        projected[i] = to_screen(project(actual));
    }

    //for (size_t i = 0; i < pts->count; i++)
    //    draw_point(projected[i], 10);

    int color = 0x22FF22;
    for (size_t fi = 0; fi < obj->faces.count; fi++) {
        Face face = obj->faces.items[fi];
        for (size_t vi = 0; vi < face.vertc; vi++) {
            ScreenVec a = projected[face.verts[vi]-1];
            ScreenVec b = projected[face.verts[(vi+1)%face.vertc]-1];
            draw_line(a, b, color);
        }
    }

    free(projected);
}

FILE *write_frame(const char *path) {
    FILE *file = fopen(path, "wb");
    fprintf(file, "P6\n");
    fprintf(file, "%d %d\n", image_dim.x, image_dim.y);
    fprintf(file, "255\n");
    fwrite(image_buffer, 1, to_buflen(image_dim), file);
    return file;
}
