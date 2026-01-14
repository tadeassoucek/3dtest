#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "vector.h"
#include "graphics.h"

extern ScreenVec image_dim;

Object obj = {0};

void create_frame(int frame, int fps, float time) {
    // clear buffer, draw object to it
    new_buffer();
    draw_object(&obj);

    // output to a file
    char buf[255];
    snprintf(buf, sizeof(buf), "out/image%03d.ppm", frame);
    FILE *f = write_frame(buf);
    fclose(f);
    //printf("> wrote %s (time %f)\n", buf, time);

    // animate
    float dt = 1./fps;
    obj.rotation.x += M_PI/2*dt;
    obj.rotation.y += M_PI/2*dt;
    obj.rotation.z += M_PI/2*dt;

    char cmd[255];
    snprintf(cmd, sizeof(cmd), "chafa \"%s\" --format symbols --align center", buf);
    (void)system(cmd);
}

int main() {
    image_dim = (ScreenVec){ 800, 800 };

    //FILE *f = fopen("res/cube.obj", "r");
    FILE *f = fopen("res/utah_teapot.obj", "r");
    obj = load_object_file(f);
    fclose(f);
    
    //obj.transform.y = 0.5;
    obj.transform.z = 5.0;
    //obj.scale = vec3_scale_s(obj.scale, 2);

    const int seconds = 4;
    const int fps = 60;
    const int total_frames = seconds * fps;
    for (int i = 0; i < total_frames; i++) {
        create_frame(i, fps, (float)i/total_frames);
    }
}
