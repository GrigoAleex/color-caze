#ifndef __PAINT_H__
#define __PAINT_H__

#include "raylib.h"
#include <stdio.h>

typedef struct {
    size_t width;
    size_t height;
    RenderTexture2D canvas;
    int pencilSize;
} WindowState;

void Start(WindowState*);
void Update(WindowState*);

#endif
