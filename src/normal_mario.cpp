#include "include/normal_mario.hpp"
#include <iostream>
#include <math.h>

NormalMario::NormalMario(Character *Ncontext, float Nscale)
    : CharacterState(10, 100, 300, Nscale) {
  Image image = LoadImage("res/sprites/mario_spritesheet.png");
  ImageResize(&image, image.width * scale, image.height * scale);
  texture = LoadTextureFromImage(image);
  UnloadImage(image);
  context = Ncontext;
  frame = {0, 0, 14 * scale, 27 * scale};
};

void NormalMario::update() {
  std::vector<Rectangle> runFrame = {

      {0, 47 * scale, 16 * scale, 27 * scale},
      {17 * scale, 48 * scale, 16 * scale, 26 * scale},
      {0, 0, 14 * scale, 27 * scale},
  };
  std::vector<Rectangle> jumpFrame = {
      {0, 75 * scale, 20 * scale, 27 * scale},
      {21 * scale, 75 * scale, 19 * scale, 26 * scale},
      {41 * scale, 75 * scale, 19 * scale, 27 * scale},
  };

  if (tY != -1) {
    if (t - tY > jT) {
      tY = -1;
      frame = {0, 0, 14 * scale, 27 * scale};
      if (left) {
        if (frame.width > 0)
          frame.width = -frame.width;
      } else if (frame.width < 0)
        frame.width = -frame.width;
    } else if (t - tY >= 0) {
      if (t - tY == 1) {
        frame = jumpFrame[0];
      } else if (t - tY == jT / 3) {
        frame = jumpFrame[1];
      } else if (t - tY == jT * 2 / 3) {
        frame = jumpFrame[2];
      }

      if (left) {
        if (frame.width > 0)
          frame.width = -frame.width;
      } else if (frame.width < 0)
        frame.width = -frame.width;

      float a = 8.0 * jH / (jT * jT), v = -4.0 * jH / jT;
      pos.y = y + v * (t - tY) + a * (t - tY) * (t - tY) / 2.0;
    }
  }

  if (tX != -1) {
    if (tY == -1) {
      if (t - tX == 15)
        tX = -1;
      else {
        if (left)
          pos.x -= speed;
        else
          pos.x += speed;
        frame = runFrame[(t - tX) / 5];
        if (left) {
          frame.width = -frame.width;
        }
      }
    } else {
      tX = -1;
      if (left)
        pos.x -= speed;
      else
        pos.x += speed;
    }
  }
}
