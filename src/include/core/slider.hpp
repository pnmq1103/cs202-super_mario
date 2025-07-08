#pragma once

#include "include/core/button.hpp"

class Slider : public Button {
private:
  Rectangle slider_bounds_ = {};
  float percentage_        = {1};
  bool is_dragging_        = {false};

public:
  ~Slider();
  Slider() = default;
  Slider(
    std::string label, std::function<void()> action, Rectangle source,
    Rectangle bounds, const std::string &path);

  void Update();
  void Draw() override;

  float GetPercentage();
};