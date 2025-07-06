#include "../include/core/setting.hpp"
#include "../include/core/application.hpp"
#include "../include/core/button.hpp"

SettingScene::SettingScene() {
  buttons_.reserve(3);
}

SettingScene::~SettingScene() {
  Application::GetInstance().ToggleCustomCursor();
}

void SettingScene::Init() {
  Application::GetInstance().ToggleCustomCursor();
}

void SettingScene::Update() {
  if (IsKeyPressed(KEY_ESCAPE)) {
    Application::ChangeScene(nullptr);
    return;
  }
}

void SettingScene::Draw() {}
