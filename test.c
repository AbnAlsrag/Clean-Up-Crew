#include <raylib/raylib.h>

int main(void) {
    InitWindow(0, 0, "test");

    Texture2D texture = LoadTexture("test.png");

    bool x = IsTextureValid(texture);

    while (!WindowShouldClose()) {
        BeginDrawing();
            DrawTexture(texture, 0, 0, WHITE);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}