#pragma once

#include "include/core/button.hpp"

class Slider : public Button {
private:
  Rectangle slider_bounds_ = {};

  float percentage_        = {1};
  float old_               = {1};
  float target_percentage_ = {1};
  float animation_speed_   = 5.0f;

  bool is_dragging_ = {false};

public:
  ~Slider();
  Slider() = default;
  Slider(
    std::string label, std::function<void()> action, Rectangle source,
    Rectangle bounds, const std::string &path, float percentage_ = 1);

  void Update();
  void Draw() override;

  float GetPercentage();
};