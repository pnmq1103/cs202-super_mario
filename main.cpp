#include "include/core/application.hpp"
#include <raylib.h>

int main() {
  App.Init();

  while (!WindowShouldClose() && !App.ShouldClose()) {
    App.Update();
    BeginDrawing();
    ClearBackground(RAYWHITE);
    App.Draw();
    EndDrawing();
  }

  CloseWindow();
  return 0;
}

/*#include "include/characters/character.hpp"
#include "include/characters/projectile_pool.hpp"
#include <raylib.h>
int main() {
  InitWindow(3000, 1000, "super mario");
  SetTargetFPS(60);
  ProjectilePool pool;
  ElectricBall ball;
  ball.Renew({100, 100}, false);
  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(BLACK);
    Rectangle rect = ball.GetRectangle();
    ball.SetFrameCount();
    ball.Update();
    ball.Draw();

    EndDrawing();
  }
  CloseWindow();
  return 0;
}*/