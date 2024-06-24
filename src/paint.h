#ifndef __PAINT_H__
#define __PAINT_H__

#include "raylib.h"
#include <stdio.h>

typedef struct {
    size_t width;
    size_t height;
    Color *matrix;
    size_t pointsCount;
    const int PIXEL_PT_RATION;
} WindowState;

enum TOOLS {
    PENCIL,
    ERASER
};

void Start(WindowState*);
void Update(WindowState*);

#endif
