#include "paint.h"

#include <raymath.h>
#include <stdlib.h>
#include <string.h>

#include "ui.h"
#include "tools.h"
#include "draw.h"
#include "colors.h"

void Start(WindowState *state)
{
    InitWindow(state->width, state->height, "Color craze");
    SetTargetFPS(60);
    state->canvas = LoadRenderTexture(state->width, state->height);
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

void DrawUI(WindowState *state, enum TOOLS *tool, const Color* colors, const int colorsCount, int* selectedColor, const Rectangle drawingBox)
{
    ClearBackground(GetColor(BACKGROUND));
    DrawRectangleRec(drawingBox, GetColor(SURFACE));
    DrawTextureRec(state->canvas.texture, (Rectangle) { 0, 0, (float)state->canvas.texture.width, (float)-state->canvas.texture.height }, (Vector2) { 0, 0 }, WHITE);
    DrawColors(state->width * 0.01, state->height - 54, selectedColor, colors, colorsCount);
    DrawTools(state->width * 0.01 + colorsCount * 38, state->height - 54, tool, &state->pencilSize);
}

void RegisterDrawnPoints(RenderTexture2D canvas, const Vector2 lastMousePosition, const Rectangle drawingBox, const int pencilSize, const Color color, const enum TOOLS tool) {
    Vector2 mousePosition = GetMousePosition();
    Color pointColor = tool == ERASER ? GetColor(SURFACE) : color;
    Rectangle cursorBoundingBox = {
        drawingBox.x + pencilSize, 
        drawingBox.y + pencilSize,
        drawingBox.width - pencilSize, 
        drawingBox.height - pencilSize
    };

    if (!CheckCollisionPointRec(mousePosition, cursorBoundingBox)) return;

    BeginTextureMode(canvas);
    DrawCircle((int)mousePosition.x, (int)mousePosition.y, pencilSize, pointColor);
    if (CheckCollisionPointRec(lastMousePosition, cursorBoundingBox)) {
        DrawLineEx(lastMousePosition, mousePosition, 2* pencilSize, pointColor);
    }
    EndTextureMode();
}

void Update(WindowState *state) {
    const Rectangle drawingBox = {state->width * 0.01, 16, state->width * 0.98, state->height - 80};     
    const Color colors[] = { BLACK, RAYWHITE, GRAY, RED, YELLOW, GREEN, BLUE, PINK, PURPLE, ORANGE };
    const int colorsCount = sizeof colors / sizeof (struct Color);
    enum TOOLS tool = PENCIL;
    int selectedColor = 0;
    Vector2 lastMousePosition = GetMousePosition();

    while (!WindowShouldClose())
    {
        
        Vector2 mousePos = GetMousePosition();

        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) || (GetGestureDetected() == GESTURE_DRAG)) {
            RegisterDrawnPoints(state->canvas, lastMousePosition, drawingBox, state->pencilSize, colors[selectedColor], tool);
        }

        BeginDrawing();
        DrawUI(state, &tool, colors, colorsCount, &selectedColor, drawingBox);
        EndDrawing();

        lastMousePosition = mousePos;
    }

    UnloadRenderTexture(state->canvas);  
    CloseWindow();
}
