#include <memory>
#include <raylib.h>
#include <utility>

#include "include//core/resource_manager.hpp"
#include "include/core/application.hpp"
#include "include/core/media.hpp"
#include "include/core/menu.hpp"
#include "include/core/scene.hpp"

Application &App = Application::GetInstance();

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
  // resource manager for testing
  // ResManager res;
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
    App.GetMedia().SaveMusicState();
    App.GetMedia().StopMusic();
    App.scene_manager_.Push(std::move(new_scene));
    App.scene_manager_.Init();
  } else {
    App.GetMedia().StopMusic();
    App.previous_scene_ = App.scene_manager_.Top().Type();
    App.scene_manager_.Pop();
    if (App.scene_manager_.Size() == App.GetMedia().MusicStateSize())
      App.GetMedia().LoadMusicState();
    if (App.scene_manager_.Top().Type() == SceneType::Menu)
      App.scene_manager_.Top().Resume();
  }
}

void Application::Close() {
  App.exit_window_ = true;
}

void Application::ToggleCustomCursor() {
  App.cursor_hidden_ = !App.cursor_hidden_;
}

SceneType Application::PreviousScene() {
  return App.previous_scene_;
}

Media &Application::GetMedia() {
  return media_;
}

bool Application::ShouldClose() {
  return exit_window_;
}
