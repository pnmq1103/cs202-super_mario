#pragma once
#include <memory>
#include <raylib.h>

#include "media.hpp"
#include "scene.hpp"
#include "scene_stack.hpp"

class Application {
private:
  static constexpr int screenWidth  = {256 * 4};
  static constexpr int screenHeight = {240 * 4};

  Image icon_ = {};
  SceneStack scene_manager_;
  Media media_;
  Texture cursor_ = {};

private:
  Application(); // Singleton constructor should be private

public:
  ~Application();

  void Init();
  void Update();
  void Draw();

  static Application &GetInstance();
  static void ChangeScene(std::unique_ptr<Scene> new_scene);
  Media &GetMedia();
};
