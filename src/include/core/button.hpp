#pragma once
#include <functional>
#include <raylib.h>
#include <string>

class Button {
private:
  std::function<void()> action_ = {nullptr};

  std::string label_ = {};
  Rectangle bounds_  = {};
  Rectangle source_  = {};
  Texture icon_      = {};

public:
  ~Button();
  Button() = default;
  Button(
    std::function<void()> action, Rectangle source, Rectangle bounds,
    const std::string &path);

  void Update();
  void Draw();
};
