#include "paint.h"

#include <raymath.h>
#include <stdlib.h>
#include "ui.h"
#include "colors.h"

void Start(WindowState *state)
{
    InitWindow(state->width, state->height, "Chad Paint");
    SetTargetFPS(144);

    state->points = (Point **)calloc(sizeof(Point*), state->width * state->height / state->PIXEL_PT_RATION);
    state->pointsCount = 0;
}

Point* point_create(Vector2 pos, Color color) {
    Point *point = (Point *)malloc(sizeof(Point));
    point->position = pos;
    point->color = color;
    return point;
}

void DrawPoints(Point **points, size_t pointsCount, size_t size)
{
    for (size_t i = 0; i < pointsCount; i++)
    {
        DrawRectangleV(points[i]->position, (Vector2){size, size}, points[i]->color);
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

void RegisterDrawnPoints(Point **points, size_t *pointsCount, Vector2* lastMousePosition, Rectangle drawingBox, const int pointSize, const Color color) {
    Vector2 mousePosition = GetMousePosition();
    drawingBox.width -= pointSize;
    drawingBox.height -= pointSize;
    
    if (!CheckCollisionPointRec(mousePosition, drawingBox)) return;

    float distance = Vector2Distance(*lastMousePosition, mousePosition);
    if (distance > 0) {
        int steps = (int)(distance / pointSize);
        Vector2 stepVector = Vector2Scale(Vector2Normalize(Vector2Subtract(mousePosition, *lastMousePosition)), pointSize);

        for (int i = 0; i < steps; i++) {
            *lastMousePosition = Vector2Add(*lastMousePosition, stepVector);
            points[(*pointsCount)++] = point_create(*lastMousePosition, color);
        }
    }

    points[(*pointsCount)++] = point_create(*lastMousePosition, color);
    *lastMousePosition = mousePosition;
}


void Update(WindowState *state) {
    const Rectangle drawingBox = {state->width * 0.01, 16, state->width * 0.98, state->height - 80}; 
    Vector2 lastMousePosition = {0, 0};
    
    const Color colors[] = { BLACK, RAYWHITE, GRAY, RED, YELLOW, GREEN, BLUE, PINK, PURPLE, ORANGE };
    const int colorsCount = sizeof colors / sizeof (struct Color);
    int selectedColor = 0;


    while (!WindowShouldClose())
    {
         if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            lastMousePosition = GetMousePosition();
        }

        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            RegisterDrawnPoints(state->points, &state->pointsCount, &lastMousePosition, drawingBox, state->PIXEL_PT_RATION, colors[selectedColor]);
        }

        BeginDrawing();
        DrawUI(state, colors, colorsCount, &selectedColor, drawingBox);
        DrawPoints(state->points, state->pointsCount, state->PIXEL_PT_RATION);
        EndDrawing();
    }

    CloseWindow();
}
