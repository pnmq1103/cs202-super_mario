#pragma once
#include <memory>
#include <raylib.h>

#include "menu.hpp"
#include "scene.hpp"

class Application {
private:
  const int screenWidth = 256 * 4, screenHeight = 240 * 4;
  Image icon_;
  std::unique_ptr<Scene> cur_scene_;

public:
  Application();
  ~Application();

  static Application &GetInstance();
  static void ChangeScene(std::unique_ptr<Scene> new_scene);

  void Init();
  void Update();
  void Draw();
};
