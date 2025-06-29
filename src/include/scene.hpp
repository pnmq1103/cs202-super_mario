#pragma once
#include <raylib.h>

class Scene {
protected:
  static constexpr int screenWidth  = 256 * 4;
  static constexpr int screenHeight = 240 * 4;
  Camera2D camera                   = {0};

public:
  virtual ~Scene()      = default;
  virtual void Init()   = 0;
  virtual void Update() = 0;
  virtual void Draw()   = 0;
};
