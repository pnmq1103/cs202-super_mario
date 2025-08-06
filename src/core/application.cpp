#include <raylib.h>
#include <stdexcept>

#include "include/core/application.hpp"
#include "include/core/media.hpp"
#include "include/core/menu.hpp"
#include "include/core/sprite_manager.hpp"

Application &App = Application::Instance();

Application::Application() {
  SetTargetFPS(60);
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
  InitWindow(256 * 4, 240 * 4, "Mario");
  SetExitKey(KEY_ZERO);
  InitAudioDevice();
  icon_image_ = LoadImage("res/app_icon.png");
  SetWindowIcon(icon_image_);
  cursor_texture_ = LoadTexture("res/sprites/cursors/hand_point.png");
  HideCursor();
  res_manager_.Init();
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
  if (cursor_hidden_ == false)
    DrawTextureRec(cursor_texture_, {0, 0, 64, 64}, GetMousePosition(), WHITE);

  if (phase_ == TransitionState::InProgress) {
    transition_.Draw(GetFrameTime());
    if (transition_.Done())
      phase_ = TransitionState::Done;
  }
}

bool Application::ShouldClose() {
  return exit_window_;
}

Application &Application::Instance() {
  static Application instance;
  return instance;
}

void Application::Run() {
  Init();
  while (!WindowShouldClose() && !ShouldClose()) {
    Update();
    BeginDrawing();
    ClearBackground(RAYWHITE);
    Draw();
    EndDrawing();
  }
  CloseWindow();
}

SceneType Application::PreviousScene() {
  return App.previous_scene_;
}

SpriteManager &Application::Resource() {
  return res_manager_;
}

Media &Application::Media() {
  return media_;
}

void Application::AddScene(std::unique_ptr<Scene> new_scene, bool hide_prev) {
  if (new_scene == nullptr)
    throw std::runtime_error("add invalid scene");

  App.Media().SaveMusicState();
  App.Media().StopMusic();
  if (hide_prev)
    App.scene_manager_.Top().SetVisible(false);
  App.previous_scene_ = App.scene_manager_.Top().Type();
  App.scene_manager_.Push(std::move(new_scene));
  App.scene_manager_.Init();

  if (App.scene_manager_.Top().Type() != SceneType::Pause) {
    App.transition_.Reset();
    App.phase_ = TransitionState::InProgress;
  }
}

void Application::RemoveScene(size_t count) {
  if (count > App.scene_manager_.Size())
    throw std::runtime_error("pop count exceeds list size");

  App.Media().StopMusic();
  for (size_t i = 0; i < count; ++i) {
    App.previous_scene_ = App.scene_manager_.Top().Type();
    App.scene_manager_.Pop();
    if (App.scene_manager_.Size() == App.Media().MusicStateSize())
      App.Media().LoadMusicState();
  }
  App.scene_manager_.Top().SetVisible(true);
  App.scene_manager_.Top().Resume();
}

void Application::RemoveSceneUntil(SceneType type) {
  App.Media().StopMusic();

  while (App.scene_manager_.Size()) {
    if (App.scene_manager_.Top().Type() == type)
      break;

    App.previous_scene_ = App.scene_manager_.Top().Type();
    App.scene_manager_.Pop();
    if (App.scene_manager_.Size() == App.Media().MusicStateSize())
      App.Media().LoadMusicState();
  }

  App.scene_manager_.Top().SetVisible(true);
  App.scene_manager_.Top().Resume();
}

void Application::ToggleCustomCursor() {
  App.cursor_hidden_ = !App.cursor_hidden_;
}

void Application::Close() {
  App.exit_window_ = true;
}