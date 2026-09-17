#include <iostream>
#include "raylib.h"

int main() {
    std::cout << "Booting Card Game Suite..." << std::endl;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_HIGHDPI | FLAG_VSYNC_HINT);

    int windowedWidth = 1280;
    int windowedHeight = 720;

    InitWindow(windowedWidth, windowedHeight, "Card Game Suite - Setup Test");
    SetWindowMinSize(960, 540);
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        // Safe fullscreen toggle that respects native macOS maximize/fullscreen states
        if (IsKeyPressed(KEY_F) || IsKeyPressed(KEY_F11)) {
            if (IsWindowState(FLAG_WINDOW_MAXIMIZED)) {
                ClearWindowState(FLAG_WINDOW_MAXIMIZED);
                ClearWindowState(FLAG_WINDOW_UNDECORATED);
                SetWindowSize(windowedWidth, windowedHeight);
            } else {
                windowedWidth = GetScreenWidth();
                windowedHeight = GetScreenHeight();
                SetWindowState(FLAG_WINDOW_MAXIMIZED | FLAG_WINDOW_UNDECORATED);
            }
        }

        int currentWidth = GetScreenWidth();
        int currentHeight = GetScreenHeight();

        BeginDrawing();
        ClearBackground(GetColor(0x1B4D3EFF)); // Table-felt green

        const char* titleText = "Card Game Suite: Window Management";
        int titleWidth = MeasureText(titleText, 24);

        DrawText(titleText, (currentWidth - titleWidth) / 2, currentHeight / 2 - 30, 24, RAYWHITE);
        DrawText("Press [F] or use macOS Green Button to Toggle Fullscreen", 
                 (currentWidth - MeasureText("Press [F] or use macOS Green Button to Toggle Fullscreen", 16)) / 2, 
                 currentHeight / 2 + 10, 16, LIGHTGRAY);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}