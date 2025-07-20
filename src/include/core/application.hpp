#pragma once
#include <memory>
#include <raylib.h>
#include <stack>

#include "include/core/media.hpp"
#include "include/core/resource_manager.hpp"
#include "include/core/scene.hpp"
#include "include/core/scene_stack.hpp"

class Application {
private:
  static constexpr int screenWidth  = {256 * 4};
  static constexpr int screenHeight = {240 * 4};

  SceneStack scene_manager_;
  SceneType previous_scene_ = {Exit};

  Image icon_image_ = {};
  Media media_;

  Texture cursor_texture_ = {};
  bool cursor_hidden_     = {false}; // Custom cursor

  bool exit_window_ = {false};

  ResManager res_manager_;

private:
  Application(); // Singleton constructor should be private

public:
  ~Application();

  void Init();
  void Update();
  void Draw();

  static Application &Instance();
  static void ChangeScene(std::unique_ptr<Scene> new_scene);
  static void Close();
  static void ToggleCustomCursor();
  static SceneType PreviousScene();

  Media &Media();
  ResManager &Resource();
  bool ShouldClose();
};

extern Application &App;
