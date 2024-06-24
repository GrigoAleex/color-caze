#include "src/paint.h"

#define UI_WIDTH 1280
#define UI_HEIGHT 720

int main(void)
{
    WindowState state = {
        .width = UI_WIDTH,
        .height = UI_HEIGHT,
        .PIXEL_PT_RATION = 5
    };

    Start(&state);
    Update(&state);

    return 0;
}
