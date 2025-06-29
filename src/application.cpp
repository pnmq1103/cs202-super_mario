#include "include/application.hpp"
#include "include/menu.hpp"

Application::Application() {
  SetTargetFPS(120);
  SetConfigFlags(FLAG_MSAA_4X_HINT);
  icon_ = LoadImage("res/app_icon.png");
  scene_manager_.Push(std::make_unique<MenuScene>());
}

Application::~Application() {
  UnloadImage(icon_);
}

void Application::Init() {
  InitWindow(screenWidth, screenHeight, "Mario");
  SetWindowIcon(icon_);
  SetExitKey(KEY_ZERO);
  scene_manager_.Init();
}

void Application::Update() {
  scene_manager_.Update();
}

void Application::Draw() {
  scene_manager_.Draw();
  DrawFPS(15, 15);
}

Application &Application::GetInstance() {
  static Application instance;
  return instance;
}

void Application::ChangeScene(std::unique_ptr<Scene> new_scene) {
  if (new_scene != nullptr)
    GetInstance().scene_manager_.Push(std::move(new_scene));
  else
    GetInstance().scene_manager_.Pop();
}
