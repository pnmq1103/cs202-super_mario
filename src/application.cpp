#include <memory>
#include <raylib.h>
#include <utility>

#include "include/application.hpp"
#include "include/media.hpp"
#include "include/menu.hpp"
#include "include/scene.hpp"

Application::Application() {
  SetTargetFPS(120);
  SetConfigFlags(FLAG_MSAA_4X_HINT);
  scene_manager_.Push(std::make_unique<MenuScene>());
}

Application::~Application() {
  UnloadTexture(cursor_);
  UnloadImage(icon_);
  CloseAudioDevice();
  CloseWindow();
}

void Application::Init() {
  InitWindow(screenWidth, screenHeight, "Mario");
  InitAudioDevice();
  SetExitKey(KEY_ZERO);
  icon_ = LoadImage("res/app_icon.png");
  SetWindowIcon(icon_);
  cursor_ = LoadTexture("res/sprites/cursors/hand_point.png");
  HideCursor();
  media_.Init();
  scene_manager_.Init();
}

void Application::Update() {
  scene_manager_.Update();
  media_.UpdateMusic();
}

void Application::Draw() {
  scene_manager_.Draw();
  DrawFPS(15, 15);
  DrawTextureRec(cursor_, {0, 0, 64, 64}, GetMousePosition(), WHITE);
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

bool Application::ShouldClose() {
  return exit_window_;
}

Media &Application::GetMedia() {
  return media_;
}
