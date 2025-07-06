#include "../include/core/setting.hpp"
#include "../include/core/application.hpp"
#include "../include/core/button.hpp"

SettingScene::SettingScene() {
  buttons_.reserve(3);
}

SettingScene::~SettingScene() {
  App.ToggleCustomCursor();
}

void SettingScene::Init() {
  App.ToggleCustomCursor();
}

void SettingScene::Update() {
  if (IsKeyPressed(KEY_ESCAPE)) {
    App.ChangeScene(nullptr);
    return;
  }
}

void SettingScene::Draw() {}
