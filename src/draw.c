#include "draw.h"
#include "colors.h"

void DrawToolHighlight (const int x, const int y, const int size, const Color color) {
    DrawRectangleLines(x, y, size, size, color);
}

void DrawColors(int x, int y, int *selectedColor, const Color* colors, const int colorsCount) {
    int colorSize = 32;
    for (size_t i = 0; i < colorsCount; i++)
    {
        Rectangle colorRect = {x, y, colorSize, colorSize};
        DrawRectangleRec(colorRect, colors[i]);
        
        if (*selectedColor == i) {
            DrawToolHighlight(x - 2, y - 2, colorSize + 4, colors[i]);
        }

        if (CheckCollisionPointRec(GetMousePosition(), colorRect)) {
            DrawToolHighlight(x - 2, y - 2, colorSize + 4, colors[i]);
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                *selectedColor = i;
            }
        }
        
        x += 36;
    }
}

void DrawTools(int x, int y, enum TOOLS *tool, int *pencilSize) {
    const int toolSize = 36;
    const Color toolColor = GetColor(SURFACE);
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
        DrawToolHighlight(x - 2, y - 2, toolSize + 4, *tool == PENCIL ? toolSelected : toolHighlight);
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            *tool = PENCIL;
        }
    }

    if (CheckCollisionPointRec(GetMousePosition(), eraserRect)) {
        DrawToolHighlight(eraserRect.x - 2, y - 2, toolSize + 4, *tool == ERASER ? toolSelected : toolHighlight);
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            *tool = ERASER;
        }
    }

    if (CheckCollisionPointRec(GetMousePosition(), minusRect)) {
        DrawToolHighlight(minusRect.x - 2, y - 2, toolSize + 4, *tool == ERASER ? toolSelected : toolHighlight);
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            *pencilSize = *pencilSize > 1 ? *pencilSize - 1 : *pencilSize;
        }
    }

    if (CheckCollisionPointRec(GetMousePosition(), plusRect)) {
        DrawToolHighlight(plusRect.x - 2, y - 2, toolSize + 4, *tool == ERASER ? toolSelected : toolHighlight);
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            *pencilSize = *pencilSize < 25 ? *pencilSize + 1 : *pencilSize;
        }
    }
}


