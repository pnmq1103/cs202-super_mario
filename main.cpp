#include <raylib.h>

#include "src/include/application.hpp"

int main() {
  Application &app = Application::GetInstance();
  app.Init();

  while (!WindowShouldClose()) {
    app.Update();

    BeginDrawing();
    ClearBackground(RAYWHITE);

    app.Draw();

    EndDrawing();
  }
  CloseWindow();
  return 0;
}