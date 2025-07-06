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

  Image icon_image_ = {};
  SceneStack scene_manager_;
  Media media_;
  Texture cursor_texture_ = {};

  bool exit_window_   = {false};
  bool cursor_hidden_ = {false}; // Custom cursor

private:
  Application(); // Singleton constructor should be private

public:
  ~Application();

  void Init();
  void Update();
  void Draw();

  static Application &GetInstance();
  static void ChangeScene(std::unique_ptr<Scene> new_scene);
  static void Close();
  static void ToggleCustomCursor();

  Media &GetMedia();
  bool ShouldClose();
};

extern Application &App;
