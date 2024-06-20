#include "paint.h"

#include <raymath.h>
#include <stdlib.h>
#include "ui.h"
#include "colors.h"

void Start(WindowState *state)
{
    
    InitWindow(state->width, state->height, "Chad Paint");
    SetTargetFPS(144);

    state->points = (Vector2 *)malloc(sizeof(Vector2) * state->width * state->height / state->PIXEL_PT_RATION);
    state->pointsCount = 0;
}

void DrawPoints(Vector2 *points, size_t pointsCount, Color color, size_t size)
{
    for (size_t i = 0; i < pointsCount; i++)
    {
        DrawRectangleV(points[i], (Vector2){size, size}, color);
    }
}
void DrawColorHighlight (const int x, const int y, const int size, const Color color) {
    DrawRectangleLines(x, y, size, size, color);
}

void DrawColors(int x, int y, int *selectedColor, const Color* colors, const int colorsCount) {
    int colorSize = 24;
    for (size_t i = 0; i < colorsCount; i++)
    {
        Rectangle colorRect = {x, y, colorSize, colorSize};
        DrawRectangleRec(colorRect, colors[i]);
        
        if (*selectedColor == i) {
            DrawColorHighlight(x - 2, y - 2, colorSize + 4, colors[i]);
        }

        if (CheckCollisionPointRec(GetMousePosition(), colorRect)) {
            DrawColorHighlight(x - 2, y - 2, colorSize + 4, colors[i]);
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                *selectedColor = i;
            }
        }
        
        x += 32;
    }
}

void DrawUI(WindowState *state, const Color* colors, const int colorsCount, int* selectedColor, const Rectangle drawingBox)
{
    ClearBackground(GetColor(BACKGROUND));
    DrawRectangleRec(drawingBox, GetColor(SURFACE));
    DrawColors(state->width * 0.01, state->height - 54, selectedColor, colors, colorsCount);
}

void Update(WindowState *state) {
    Vector2 lastMousePosition = {0, 0};
    bool firstPoint = true;
    
    int selectedColor = 0;
    const Rectangle drawingBox = {state->width * 0.01, 16, state->width * 0.98, state->height - 80}; 
    const Color colors[] = { BLACK, RAYWHITE, GRAY, RED, YELLOW, GREEN, BLUE, PINK, PURPLE, ORANGE };
    const int colorsCount = sizeof colors / sizeof (struct Color);

    while (!WindowShouldClose())
    {
         if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            firstPoint = true;
        }

        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
        {
            Vector2 mousePosition = GetMousePosition();
            if (firstPoint)
            {
                firstPoint = false;
            }
            else
            {
                float distance = Vector2Distance(lastMousePosition, mousePosition);
                if (distance > state->PIXEL_PT_RATION)
                {
                    int steps = distance / state->PIXEL_PT_RATION;
                    Vector2 stepVector = Vector2Scale(Vector2Normalize(Vector2Subtract(mousePosition, lastMousePosition)), state->PIXEL_PT_RATION);

                    for (int i = 0; i < steps; i++)
                    {
                        lastMousePosition = Vector2Add(lastMousePosition, stepVector);
                        state->points[state->pointsCount] = lastMousePosition;
                        state->pointsCount++;
                    }
                }
            }
            state->points[state->pointsCount] = mousePosition;
            lastMousePosition = mousePosition;
            state->pointsCount++;
        }
        
        BeginDrawing();
        DrawUI(state, colors, colorsCount, &selectedColor, drawingBox);
        DrawPoints(state->points, state->pointsCount, colors[selectedColor], state->PIXEL_PT_RATION);
        EndDrawing();
    }

    CloseWindow();
}
