#include <stdio.h>
#include <string.h>
#include "./graphics.h"

// parse a given .OBJ file
Object load_object_file(FILE *file) {
    Object obj = {0};
    static size_t total_id = 0;
    obj.id = total_id++;

    char name[GR_OBJ_FILE_BUFSIZE];
    char buf[GR_OBJ_FILE_BUFSIZE];

    // we use these vectors for calculating the origin, i.e. the 
    // midpoint of the object
    Vec3 min = {0};
    Vec3 max = {0};

    // read line
    while (fgets(buf, GR_OBJ_FILE_BUFSIZE, file)) {
        if (buf[0] == 'o') {
            sscanf(buf, "o %s", name);
            obj.name = strdup(name);
        }
        else if (buf[0] == 'v' && buf[1] == ' ') {
            float x, y, z;
            sscanf(buf, "v %f %f %f", &x, &y, &z);
            //printf("creating point at %f %f %f\n", x, y, z);
            dl_append(obj.verts, ((Vec3){ x, y, z }));

            // update min/max vectors accordingly
            if (x < min.x) min.x = x;
            if (x > max.x) max.x = x;
            if (y < min.y) min.y = y;
            if (y > max.y) max.y = y;
            if (z < min.z) min.z = z;
            if (z > max.z) max.z = z;
        }
        else if (buf[0] == 'f') {
            // we assume the face consists of <=4 vertices
            int verts[4];
            int read = sscanf(buf, "f %d/%*d/%*d %d/%*d/%*d %d/%*d/%*d %d/%*d/%*d", &verts[0], &verts[1], &verts[2], &verts[3]);
            Face face = {
                .verts = malloc(read * sizeof(Face)),
                .vertc = read
            };
            for (size_t i = 0; i < read; i++) {
                face.verts[i] = verts[i]-1;
            }
            dl_append(obj.faces, face);

            if (0) {
                printf("\t> created face (%d verts):", read);
                for (size_t i = 0; i < read; i++) {
                    Vec3 v = obj.verts.items[face.verts[i]-1];
                    printf(" [%f;%f;%f]", v.x, v.y, v.z);
                }
                putchar('\n');
            }
        }
    }

    Vec3 origin = {
        (max.x + min.x) / 2,
        (max.y + min.y) / 2,
        (max.z + min.z) / 2,
    };
    obj.scale = (Vec3){ 1, 1, 1 };

    for (size_t i = 0; i < obj.verts.count; i++)
        obj.verts.items[i] = vec3_sub(obj.verts.items[i], origin);

    printf("> loaded object \"%s\"#%zu with %zu vertices and %zu faces\n", obj.name, obj.id, obj.verts.count, obj.faces.count);
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

Color to_color(colorhex color) {
    return (Color){
        (color & 0xFF0000) >> 16,
        (color & 0x00FF00) >> 8,
        (color & 0x0000FF),
    };
}

// color is RGB
void draw_pixel(size_t x, size_t y, Color color) {
    if (x >= image_dim.x || y >= image_dim.y) return;
    size_t i = image_dim.x * 3 * y + 3 * x;
    // red
    image_buffer[i]   = color.r;
    // green
    image_buffer[i+1] = color.g;
    // blue
    image_buffer[i+2] = color.b;
}

// this function is very silly and doesn't implement any anti-aliasing
// oh well.
void draw_line(ScreenVec a, ScreenVec b, Color color) {
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
void draw_point(ScreenVec p, screenint s, Color color) {
    screenint d = s/2;
    if (p.x > image_dim.x || p.y > image_dim.y)
        return;
    for (int j = -d; j < d; j++)
        for (int i = -d; i < d; i++)
            draw_pixel(p.x + i, p.y + j, color);
}

void draw_object(Object *obj, colorhex vert_color, colorhex line_color) {
    ScreenVec *projected_verts = malloc(obj->verts.count * sizeof(ScreenVec));
    for (size_t i = 0; i < obj->verts.count; i++) {
        Vec3 projected = obj->verts.items[i];
        projected = vec3_scale(projected, obj->scale);
        projected = vec3_rotate(projected, obj->rotation);
        projected = vec3_add(projected, obj->transform);
        projected_verts[i] = to_screen(project(projected), image_dim);
    }

    Color vert_color_t = to_color(vert_color);
    Color line_color_t = to_color(line_color);

    // draw vertices
    if (vert_color)
        for (size_t i = 0; i < obj->verts.count; i++)
            draw_point(projected_verts[i], 10, vert_color_t);

    // draw lines
    for (size_t fi = 0; fi < obj->faces.count; fi++) {
        Face face = obj->faces.items[fi];
        for (size_t vi = 0; vi < face.vertc; vi++) {
            ScreenVec a = projected_verts[face.verts[vi]];
            // next index (i+1 or 0)
            size_t nexti = vi == face.vertc-1 ? 0 : vi+1;
            ScreenVec b = projected_verts[face.verts[nexti]];
            draw_line(a, b, line_color_t);
        }
    }

    free(projected_verts);
}

FILE *write_frame(const char *path) {
    FILE *file = fopen(path, "wb");
    // magic code
    fprintf(file, "P6 ");
    // resolution
    fprintf(file, "%d %d ", image_dim.x, image_dim.y);
    // color depth
    fprintf(file, "255 ");
    // write buffer
    fwrite(image_buffer, 1, to_buflen(image_dim), file);
    return file;
}
