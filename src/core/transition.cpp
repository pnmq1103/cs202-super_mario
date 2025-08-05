#include <raylib.h>

#include "include/core/constants.hpp"
#include "include/core/transition.hpp"

void Transition::InTransition(float dt) {
  bar_x_ -= speed_ * dt;
  if (bar_x_ <= 0) {
    bar_x_ = 0;
    return;
  }
  Rectangle bar = {0, 0, bar_x_, constants::screenHeight};
  DrawRectangleRec(bar, WHITE);
}

void Transition::OutTransition(float dt) {
  bar_x_ -= speed_ * dt;
  if (bar_x_ <= 0) {
    bar_x_ = 0;
    return;
  }
  Rectangle bar
    = {bar_x_, 0, constants::screenWidth - bar_x_, constants::screenHeight};
  DrawRectangleRec(bar, WHITE);
}

void Transition::Reset() {
  bar_x_ = constants::screenWidth;
}

bool Transition::Done() {
  return bar_x_ == 0;
}
