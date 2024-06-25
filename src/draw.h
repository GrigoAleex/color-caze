#ifndef __DRAW_H__
#define __DRAW_H__

#include <raylib.h>
#include <stdio.h>

#include "tools.h"

/** DRAW A RECTANGLE HIGHLIGHT */
void DrawToolHighlight (const int x, const int y, const int size, const Color color);

/** DRAW THE COLOR SELECTOR */
void DrawColors(int x, int y, int *selectedColor, const Color* colors, const int colorsCount);

/** DRAW THE TOOLBARS */
void DrawTools(int x, int y, enum TOOLS *tool, int *pencilSize);

#endif
