#include "paint.h"

#include <raymath.h>
#include <stdlib.h>
#include <string.h>
#include "ui.h"
#include "colors.h"

void Start(WindowState *state)
{
    InitWindow(state->width, state->height, "Color craze");
    SetTargetFPS(144);

    state->matrix = (Color *)calloc(sizeof(Color), state->width * state->height);
    for (size_t i = 0; i < state->width * state->height; i++) {
        state->matrix[i] = GetColor(SURFACE);
    }
    
    state->pointsCount = 0;
}

void DrawPoints(const Color *matrix, const size_t width, const size_t height, const Rectangle drawingBox)
{
    for (size_t i = 0; i < drawingBox.width; i++) {
        for (size_t j = 0; j < drawingBox.height; j++) {
            DrawRectangle(drawingBox.x + i, drawingBox.y + j, 1, 1, matrix[i + j * (int)drawingBox.width + 1]);
        }
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

void DrawTools(int x, int y, enum TOOLS *tool) {
    const int toolSize = 24;
    const Color toolColor = GetColor(BACKGROUND);
    const Color toolHighlight = GetColor(SURFACE);
    const Color toolSelected = RAYWHITE;

    Rectangle pencilRect = {x, y, toolSize, toolSize};
    Rectangle eraserRect = {x + 32, y, toolSize, toolSize};

    DrawRectangleRec(pencilRect, *tool == PENCIL ? toolSelected : toolHighlight);
    DrawRectangleRec(eraserRect, *tool == ERASER ? toolSelected : toolHighlight);

    if (CheckCollisionPointRec(GetMousePosition(), pencilRect)) {
        DrawColorHighlight(x - 2, y - 2, toolSize + 4, *tool == PENCIL ? toolSelected : toolHighlight);
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            *tool = PENCIL;
        }
    }

    if (CheckCollisionPointRec(GetMousePosition(), eraserRect)) {
        DrawColorHighlight(x - 2 + 32, y - 2, toolSize + 4, *tool == ERASER ? toolSelected : toolHighlight);
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            *tool = ERASER;
        }
    }
}

void DrawUI(WindowState *state, enum TOOLS *tool, const Color* colors, const int colorsCount, int* selectedColor, const Rectangle drawingBox)
{
    ClearBackground(GetColor(BACKGROUND));
    DrawColors(state->width * 0.01, state->height - 54, selectedColor, colors, colorsCount);
    DrawTools(state->width * 0.01 + colorsCount * 34, state->height - 54, tool);
}

int getIndexVec2(const Vector2 vec, const Rectangle drawingBox) {
    return (int)(vec.y - drawingBox.y) * (int)drawingBox.width + (int)(vec.x - drawingBox.x);
}

void FillPoint(Color *matrix, const Rectangle drawingArea, const int centerIndex, const int pointSize, const Color color) {
    u_int8_t diametre;
    for (diametre = 1; diametre * diametre < pointSize; diametre += 2) {}
    
    for (int i = 0; i < diametre; i++) {
        for (int j = 0; j < diametre; j++) {
            int x = i - diametre / 2;
            int y = j - diametre / 2;
            int index = centerIndex + x + y * (int) drawingArea.width;
            if (index >= 0 && index < (int)(drawingArea.width * drawingArea.height)) {
                matrix[index] = color;
            }
        }
    }
}

void RegisterDrawnPoints(Color *matrix, Vector2* lastMousePosition, const Rectangle drawingBox, const int pointSize, const Color color, const enum TOOLS tool) {
    Vector2 mousePosition = GetMousePosition();
    Color pointColor = tool == ERASER ? GetColor(SURFACE) : color;
    
    if (!CheckCollisionPointRec(mousePosition, drawingBox)) return;

    float distance = Vector2Distance(*lastMousePosition, mousePosition);
    if (distance > 0) {
        int steps = 2.6 * (int)(distance / (pointSize));
        Vector2 stepVector = Vector2Scale(Vector2Normalize(Vector2Subtract(mousePosition, *lastMousePosition)), pointSize / 2);

        for (int i = 0; i < steps; i++) {
            *lastMousePosition = Vector2Add(*lastMousePosition, stepVector);
            FillPoint(matrix, drawingBox, getIndexVec2(*lastMousePosition, drawingBox), pointSize, pointColor);
        }
    }

    FillPoint(matrix, drawingBox, getIndexVec2(*lastMousePosition, drawingBox), pointSize, pointColor);
    *lastMousePosition = mousePosition;
}


void Update(WindowState *state) {
    const Rectangle drawingBox = {state->width * 0.01, 16, state->width * 0.98, state->height - 80}; 
    Vector2 lastMousePosition = {0, 0};
    
    const Color colors[] = { BLACK, RAYWHITE, GRAY, RED, YELLOW, GREEN, BLUE, PINK, PURPLE, ORANGE };
    const int colorsCount = sizeof colors / sizeof (struct Color);
    int selectedColor = 0;

    enum TOOLS tool = PENCIL;

    while (!WindowShouldClose())
    {
         if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            lastMousePosition = GetMousePosition();
        }

        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            RegisterDrawnPoints(state->matrix, &lastMousePosition, drawingBox, state->PIXEL_PT_RATION, colors[selectedColor], tool);
        }

        BeginDrawing();
        DrawUI(state, &tool, colors, colorsCount, &selectedColor, drawingBox);
        DrawPoints(state->matrix, state->width, state->height, drawingBox);
        EndDrawing();
    }

    CloseWindow();
}
