#include "src/include/core/application.hpp"
#include "raylib.h"

int main() {
  Application &app = Application::GetInstance();
  app.Init();


  while (!WindowShouldClose() && !app.ShouldClose()) {
    app.Update();
    BeginDrawing();
    ClearBackground(RAYWHITE);
    app.Draw();
    EndDrawing();
  }

  CloseWindow();
  return 0;
}