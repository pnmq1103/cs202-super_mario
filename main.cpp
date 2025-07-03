#include "include/character.hpp"
#include "include/fireball.hpp"
#include <fstream>
#include <iostream>
#include <raylib.h>
#include <vector>

int main() {
  InitWindow(3000, 1000, "super mario");
  SetTargetFPS(60);

  ElectricBall a({200, 100}, false);
  MarioFireball b({200, 300}, false);
  EnemyFireball c({200, 700}, false);

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(BLACK);
    Rectangle rect = a.GetRectangle();
    if (rect.x > 2000)
      a.Destroy();

    a.SetFrameCount();
    a.Update();
    a.Draw();

    b.SetFrameCount();
    b.Update(200, 500);
    b.Draw();

    c.SetFrameCount();
    c.Update();
    c.Draw();

    EndDrawing();
  }

  CloseWindow();
  return 0;
}