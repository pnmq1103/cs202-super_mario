#pragma once
#include <memory>
#include <raylib.h>
#include <stack>

#include "include/core/media.hpp"
#include "include/core/scene_list.hpp"
#include "include/core/sprite_manager.hpp"

class Application {
private:
  SpriteManager res_manager_;
  Media media_;
  Image icon_image_ = {};

  SceneStack scene_manager_;
  SceneType previous_scene_ = {Exit};

  Texture cursor_texture_ = {};
  bool cursor_hidden_     = {false}; // Custom cursor

  bool exit_window_ = {false};

private:
  Application(); // Singleton constructor should be private

public:
  ~Application();
  void Run();

  // These will stay public for now for testing purposes
  void Init();
  void Update();
  void Draw();
  bool ShouldClose();

  static Application &Instance();
  static SceneType PreviousScene();
  SpriteManager &Resource();
  Media &Media();

  static void AddScene(std::unique_ptr<Scene> new_scene, bool hide_prev = true);
  static void RemoveScene(size_t count = 1);
  static void ToggleCustomCursor();
  static void Close();
};

extern Application &App;
