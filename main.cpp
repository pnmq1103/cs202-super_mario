#include "src/include/core/application.hpp"
#include "src/include/core/resource_manager.hpp"
#include <raylib.h>

int main() {
  Application &app = Application::GetInstance();
  app.Init();
  ResManager res;

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