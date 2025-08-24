#pragma once
#include <memory>
#include <raylib.h>
#include <stack>

#include "include/core/config.hpp"
#include "include/core/media.hpp"
#include "include/core/scene_list.hpp"
#include "include/core/sprite_manager.hpp"
#include "include/core/transition.hpp"

class Application {
private:
  SpriteManager res_manager_;
  Media media_;
  Image icon_image_ = {};
  Config config_;

  SceneList scene_manager_;
  SceneType previous_scene_ = {Exit};

  Texture cursor_texture_ = {};
  bool cursor_hidden_     = {false}; // Custom cursor

  bool exit_window_ = {false};

  Transition transition_;
  TransitionState phase_ = {TransitionState::InProgress};

private:
  Application();
  Application(const Application &)            = delete;
  Application &operator=(const Application &) = delete;

  void Init();
  void Update();
  void Draw();
  bool ShouldClose();

public:
  ~Application();
  void Run();

  static Application &Instance();
  static SceneType PreviousScene();
  SpriteManager &Resource();
  Media &Media();
  Config &Config();

  static void AddScene(std::unique_ptr<Scene> new_scene, bool hide_prev = true);
  static void RemoveScene(size_t count = 1);
  static void RemoveSceneUntil(SceneType type);
  static void ToggleCustomCursor();
  static void Close();
};

extern Application &App;