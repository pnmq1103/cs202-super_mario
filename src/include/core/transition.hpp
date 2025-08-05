#pragma once
#include "include/core/constants.hpp"

enum class TransitionState {
  In,
  Out,
  Done,
};

class Transition {
private:
  float duration_ = {1};
  float speed_    = {constants::screenWidth / duration_};
  float bar_x_    = {constants::screenWidth};

public:
  void InTransition(float dt);
  void OutTransition(float dt);
  void Reset();
  bool Done();
};