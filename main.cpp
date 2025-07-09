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
#include <cmath>
#include <iostream>
#include <raylib.h>
#include <vector>
int main() {
  InitWindow(3000, 1000, "super mario");
  SetTargetFPS(60);
  Character character(5);
  character.SetCharacter(MARIO, {200, 300});
  Rectangle rect1 = {2000, 400, 100, 100};

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(BLACK);
    DrawRectangleRec(rect1, BLUE);

    Rectangle rect = character.GetRectangle();
    if (rect.y + rect.height >= 500) {
      character.StopY(500);
    }
    float speed = character.GetSpeed();

    std::vector<float> box
      = {rect.x, rect.x + rect.width, rect.y, rect.y + rect.height};

    if (
      (box[1] + speed >= 2000 && box[0] + speed < 2000)
      || (box[0] + speed <= 2100 && box[1] + speed > 2100)) {
      if (box[3] > 400)
        character.StopX();
    }
    if (box[1] >= 2000 && box[1] <= 2100 + rect.width) {
      // std::cout << "meow\n";
      if (box[3] >= 400)
        character.StopY(400);
    }
    std::cout << box[1] << " " << 2000 + rect.width << '\n';
    // std::cout << rect.x << " " << rect.width << " " << speed << '\n';

    character.SetFrameCount();
    character.Update();
    character.Draw();

    if (IsKeyDown(KEY_LEFT))
      character.Run(true);
    if (IsKeyDown(KEY_RIGHT))
      character.Run(false);
    if (IsKeyDown(KEY_UP))
      character.Jump();
    if (IsKeyDown(KEY_D))
      character.Die();
    if (IsKeyDown(KEY_E))
      character.Evolve();

    EndDrawing();
  }

  CloseWindow();
  return 0;
}*/