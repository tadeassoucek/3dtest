#ifndef TEST3D_LOG_H
#define TEST3D_LOG_H
#include <stdio.h>

#define log(...) \
    do { fprintf(stdin, __VA_ARGS__) } while(0);

#endif
