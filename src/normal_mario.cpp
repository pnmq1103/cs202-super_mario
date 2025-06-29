#include <iostream>
#include <math.h>

#include "include/normal_mario.hpp"

NormalMario::NormalMario(Character *Ncontext, float Nscale)
    : CharacterState(10, 100, 300, Nscale) {
  Image image = LoadImage("res/sprites/characters/mario_starup.png");
  ImageResize(&image, image.width * scale, image.height * scale);
  texture = LoadTextureFromImage(image);
  UnloadImage(image);
  context = Ncontext;
  frame = {254 * scale, 0, 14 * scale, 27 * scale};
};

void NormalMario::update() {
  std::vector<Rectangle> runFrame = {

      {254 * scale, 47 * scale, 16 * scale, 27 * scale},
      {271 * scale, 48 * scale, 16 * scale, 26 * scale},
      {288 * scale, 47 * scale, 14 * scale, 27 * scale},
  };
  std::vector<Rectangle> jumpFrame = {
      {254 * scale, 75 * scale, 20 * scale, 27 * scale},
      {275 * scale, 76 * scale, 19 * scale, 26 * scale},
      {295 * scale, 75 * scale, 19 * scale, 27 * scale},
  };

  if (stopLeft == 1) {
    if (pos.x > stopX)
      stopLeft = 0;
  } else if (stopLeft == -1) {
    if (pos.x < stopX)
      stopLeft = 0;
  }

  float a = 8.0 * jH / (jT * jT);
  v += a;
  pos.y += v;

  float v0 = 4.0 * jH / jT;

  if (v != 0) {
    if (v >= -v0 && v < -v0 * 2 / 3) {
      frame = jumpFrame[0];
    } else if (v > -v0 * 2 / 3 && v < v0 * 2 / 3) {
      frame = jumpFrame[1];
    } else
      frame = jumpFrame[2];
  }
  if (v > 0)
    isFall = true;

  if (tX != -1) {
    if (v == 0) {
      if (t - tX == 15) {
        tX = -1;
      } else {

        frame = runFrame[(t - tX) / 5];
      }
      if (left)
        pos.x -= speed;
      else
        pos.x += speed;
    } else {
      tX = -1;
      if (left)
        pos.x -= speed;
      else
        pos.x += speed;
    }
  }

  if (left) {
    frame.width = -abs(frame.width);
  } else
    frame.width = abs(frame.width);
}
