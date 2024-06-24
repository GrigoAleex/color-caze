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
    int colorSize = 32;
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
        
        x += 36;
    }
}

void DrawTools(int x, int y, enum TOOLS *tool, int *pencilSize) {
    const int toolSize = 36;
    const Color toolColor = GetColor(BACKGROUND);
    const Color toolHighlight = GetColor(SURFACE_HIGHLIGHT);
    const Color toolSelected = RAYWHITE;

    Rectangle pencilRect = {x, y, toolSize, toolSize};
    Rectangle eraserRect = {x + 40, y, toolSize, toolSize};
    Rectangle minusRect = {x + 80, y, toolSize, toolSize};
    Rectangle plusRect = {x + 160, y, toolSize, toolSize};

    Texture2D pencilTexture = LoadTexture("resources/pencil.png");
    Texture2D eraserTexture = LoadTexture("resources/eraser.png");
    Texture2D minusTexture = LoadTexture("resources/minus.png");
    Texture2D plusTexture = LoadTexture("resources/plus.png");

    DrawRectangleRounded(pencilRect, 0.2f, 4, GetColor(SURFACE));
    DrawTextureEx(pencilTexture, (Vector2){pencilRect.x + 2, y + 2}, 0.0f, 0.5f, *tool == PENCIL ? toolSelected : toolHighlight);

    DrawRectangleRounded(eraserRect, 0.2f, 4, GetColor(SURFACE));
    DrawTextureEx(eraserTexture, (Vector2){eraserRect.x + 2, y + 2}, 0.0f, 0.5f, *tool == ERASER ? toolSelected : toolHighlight);

    DrawRectangleRounded(minusRect, 0.2f, 4, GetColor(SURFACE));
    DrawTextureEx(minusTexture, (Vector2){minusRect.x + 2, y + 2}, 0.0f, 0.5f, WHITE);

    char pencilSizeText[3];
    snprintf(pencilSizeText, 3, "%02d", *pencilSize);
    DrawText(pencilSizeText, x + 122, y + 5, 28, WHITE);

    DrawRectangleRounded(plusRect, 0.2f, 4, GetColor(SURFACE));
    DrawTextureEx(plusTexture, (Vector2){plusRect.x + 2, y + 2}, 0.0f, 0.5f, WHITE);


    if (CheckCollisionPointRec(GetMousePosition(), pencilRect)) {
        DrawColorHighlight(x - 2, y - 2, toolSize + 4, *tool == PENCIL ? toolSelected : toolHighlight);
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            *tool = PENCIL;
        }
    }

    if (CheckCollisionPointRec(GetMousePosition(), eraserRect)) {
        DrawColorHighlight(eraserRect.x - 2, y - 2, toolSize + 4, *tool == ERASER ? toolSelected : toolHighlight);
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            *tool = ERASER;
        }
    }

    if (CheckCollisionPointRec(GetMousePosition(), minusRect)) {
        DrawColorHighlight(minusRect.x - 2, y - 2, toolSize + 4, *tool == ERASER ? toolSelected : toolHighlight);
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            *pencilSize = *pencilSize > 1 ? *pencilSize - 1 : *pencilSize;
        }
    }

    if (CheckCollisionPointRec(GetMousePosition(), plusRect)) {
        DrawColorHighlight(plusRect.x - 2, y - 2, toolSize + 4, *tool == ERASER ? toolSelected : toolHighlight);
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            *pencilSize = *pencilSize < 25 ? *pencilSize + 1 : *pencilSize;
        }
    }
}

void DrawUI(WindowState *state, enum TOOLS *tool, const Color* colors, const int colorsCount, int* selectedColor, const Rectangle drawingBox)
{
    ClearBackground(GetColor(BACKGROUND));
    DrawColors(state->width * 0.01, state->height - 54, selectedColor, colors, colorsCount);
    DrawTools(state->width * 0.01 + colorsCount * 38, state->height - 54, tool, &state->pencilSize);
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

void RegisterDrawnPoints(Color *matrix, Vector2* lastMousePosition, const Rectangle drawingBox, const int pencilSize, const Color color, const enum TOOLS tool) {
    Vector2 mousePosition = GetMousePosition();
    Color pointColor = tool == ERASER ? GetColor(SURFACE) : color;
    
    if (!CheckCollisionPointRec(mousePosition, drawingBox)) return;

    float distance = Vector2Distance(*lastMousePosition, mousePosition);
    if (distance > 0) {
        int steps = (int)(distance / (pencilSize));
        Vector2 stepVector = Vector2Scale(Vector2Normalize(Vector2Subtract(mousePosition, *lastMousePosition)), pencilSize);

        for (int i = 0; i < steps; i++) {
            *lastMousePosition = Vector2Add(*lastMousePosition, stepVector);
            FillPoint(matrix, drawingBox, getIndexVec2(*lastMousePosition, drawingBox), pencilSize, pointColor);
        }
    }

    FillPoint(matrix, drawingBox, getIndexVec2(*lastMousePosition, drawingBox), pencilSize, pointColor);
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
            RegisterDrawnPoints(state->matrix, &lastMousePosition, drawingBox, state->pencilSize, colors[selectedColor], tool);
        }

        BeginDrawing();
        DrawUI(state, &tool, colors, colorsCount, &selectedColor, drawingBox);
        DrawPoints(state->matrix, state->width, state->height, drawingBox);
        EndDrawing();
    }

    CloseWindow();
}
