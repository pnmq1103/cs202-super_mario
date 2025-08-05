#pragma once

enum class TransitionState {
  InProgress,
  Done,
};

class Transition {
private:
  float timer_    = {0};
  float duration_ = {0.5f};

public:
  void Draw(float dt);
  void Reset();
  bool Done();
};