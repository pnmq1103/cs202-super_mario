#include <algorithm>
#include <raylib.h>

#include "include/core/constants.hpp"
#include "include/core/transition.hpp"

void Transition::Draw(float dt) {
  timer_             += dt;
  float t             = std::min(timer_ / duration_, 1.0f);
  float easing        = 1 - t * t * t * t;
  unsigned char alpha = static_cast<unsigned char>(easing * 256);
  Color color         = {0, 0, 0, alpha};
  DrawRectangleRec(
    {0, 0, constants::screenWidth, constants::screenHeight}, color);
}

void Transition::Reset() {
  timer_ = 0;
}

bool Transition::Done() {
  return timer_ == 1;
}
