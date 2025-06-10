#include "application.hpp"

Application::Application() {
  SetTargetFPS(60);
  SetConfigFlags(FLAG_MSAA_4X_HINT);
  icon_ = LoadImage("res/icon.png");
}

Application::~Application() { UnloadImage(icon_); }

Application &Application::GetInstance() {
  static Application instance;
  return instance;
}

void Application::Init() {
  InitWindow(screenWidth, screenHeight, "Mario");
  SetWindowIcon(icon_);
}

void Application::Update() {}

void Application::Draw() {}