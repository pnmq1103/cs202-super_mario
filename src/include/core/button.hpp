#pragma once
#include <functional>
#include <raylib.h>
#include <string>

class Button {
public:
  std::function<void()> action_ = {nullptr};

  std::string label_ = {};
  Rectangle src_     = {};
  Rectangle dst_     = {};
  Texture icon_      = {};

public:
  ~Button();
  Button() = default;
  Button(
    std::string label, std::function<void()> action, Rectangle src,
    Rectangle dst, const std::string &path);

  virtual void Update();
  virtual void Draw();

  bool Clicked();
};
