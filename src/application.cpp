#include "include/application.hpp"

Application::Application() {
  SetTargetFPS(60);
  SetConfigFlags(FLAG_MSAA_4X_HINT);
  icon_ = LoadImage("res/app_icon.png");
  cur_scene_ = std::make_unique<MenuScene>();
}

Application::~Application() { UnloadImage(icon_); }

Application &Application::GetInstance() {
  static Application instance;
  return instance;
}

void Application::Init() {
  InitWindow(screenWidth, screenHeight, "Mario");
  SetWindowIcon(icon_);
  cur_scene_->Init();
}

void Application::Update() { cur_scene_->Update(); }

void Application::Draw() { cur_scene_->Draw(); }

void Application::ChangeScene(std::unique_ptr<Scene> new_scene) {
  if (new_scene != nullptr) {
    GetInstance().cur_scene_ = std::move(new_scene);
    GetInstance().cur_scene_->Init();
  }
}
