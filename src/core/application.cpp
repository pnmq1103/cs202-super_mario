#include <memory>
#include "raylib.h"
#include <utility>

#include "../include/core/application.hpp"
#include "../include/core/media.hpp"
#include "../include/core/menu.hpp"
#include "../include/core/scene.hpp"
#include "../include//core/resource_manager.hpp"

Application::Application() {
  SetTargetFPS(120);
  SetConfigFlags(FLAG_MSAA_4X_HINT);
  scene_manager_.Push(std::make_unique<MenuScene>());
}

Application::~Application() {
  UnloadTexture(cursor_texture_);
  UnloadImage(icon_image_);
  CloseAudioDevice();
  CloseWindow();
}

void Application::Init() {
  InitWindow(screenWidth, screenHeight, "Mario");
  SetExitKey(KEY_ZERO);
  InitAudioDevice();
  icon_image_ = LoadImage("res/app_icon.png");
  SetWindowIcon(icon_image_);
  cursor_texture_ = LoadTexture("res/sprites/cursors/hand_point.png");
  HideCursor();
  media_.Init();
  scene_manager_.Init();
  //resource manager for testing
  ResManager res;
}

void Application::Update() {
  scene_manager_.Update();
  media_.UpdateMusic();
}

void Application::Draw() {
  scene_manager_.Draw();
  DrawFPS(15, 15);
  if (cursor_hidden_ == false)
    DrawTextureRec(cursor_texture_, {0, 0, 64, 64}, GetMousePosition(), WHITE);
}

Application &Application::GetInstance() {
  static Application instance;
  return instance;
}

void Application::ChangeScene(std::unique_ptr<Scene> new_scene) {

  if (new_scene != nullptr) {
    GetInstance().GetMedia().SaveMusicState();
    GetInstance().GetMedia().StopMusic();
    GetInstance().scene_manager_.Push(std::move(new_scene));
    GetInstance().scene_manager_.Init();
  } else {
    GetInstance().GetMedia().StopMusic();
    GetInstance().scene_manager_.Pop();
    GetInstance().GetMedia().LoadMusicState();
  }
}

void Application::Close() {
  Application::GetInstance().exit_window_ = true;
}

void Application::ToggleCustomCursor() {
  Application::GetInstance().cursor_hidden_
    = !Application::GetInstance().cursor_hidden_;
}

Media &Application::GetMedia() {
  return media_;
}

bool Application::ShouldClose() {
  return exit_window_;
}
