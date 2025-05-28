#include <raylib.h>

int main() {
  SetTargetFPS(60);
  SetConfigFlags(FLAG_MSAA_4X_HINT);
  InitWindow(700, 500, "Super Mario");

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawCircle(50, 50, 5, RED);
    EndDrawing();
  }
  return 0;
}