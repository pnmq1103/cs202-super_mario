#pragma once
#include <memory>
#include <raylib.h>
#include <unordered_map>

class Application {
public:
  Application();
  ~Application();

  static Application &GetInstance();

  void Init();
  void Update();
  void Draw();

private:
  const int screenWidth = 256 * 4, screenHeight = 240 * 4;
  Image icon_;
};