#ifndef __PAINT_H__
#define __PAINT_H__

#include "raylib.h"
#include <stdio.h>

typedef struct {
    size_t width;
    size_t height;
    Vector2 *points;
    size_t pointsCount;
    const int PIXEL_PT_RATION;
} WindowState;

void Start(WindowState*);
void Update(WindowState*);

#endif
