#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "vector.h"
#include "graphics.h"

extern ScreenVec image_dim;

Object obj = {0};

void create_frame(int frame, int fps) {
    float dt = 1./fps;
    new_buffer();
    draw_object(&obj);
    char buf[255];
    snprintf(buf, sizeof(buf), "out/image%03d.ppm", frame);
    FILE *f = write_frame(buf);
    fclose(f);
    printf("> wrote %s\n", buf);
    obj.rotation.y += M_PI/2*dt;
}

int main() {
    image_dim = (ScreenVec){ 800, 800 };

    FILE *f = fopen("res/utah_teapot.obj", "r");
    obj = obj_load(f);
    fclose(f);
    
    obj.transform.y = -2.0;
    obj.transform.z = 4.0;

    const int seconds = 4;
    const int fps = 60;
    for (int i = 0; i < seconds * fps; i++)
        create_frame(i, fps);
}
