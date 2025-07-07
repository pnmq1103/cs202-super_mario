/*#include "src/include/core/application.hpp"
#include <raylib.h>

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
}*/
#include "include/characters/character.hpp"
#include "include/characters/fireball.hpp"
#include <fstream>
#include <iostream>
#include <raylib.h>
#include <vector>
int main() {
  InitWindow(3000, 1000, "super mario");
  SetTargetFPS(60);

  ElectricBall ball({100, 300}, false);
  bool renew = false;
  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(BLACK);

    if (ball.IsDestroyed())
      ball.Renew({100, 300}, false);
    ball.SetFrameCount();
    ball.Update();
    ball.Draw();
    Rectangle rect = ball.GetRectangle();
    std::cout << rect.x << '\n';
    if (rect.x >= 2000) {
      ball.Destroy();
    }
    EndDrawing();
  }
  CloseWindow();
  return 0;
}