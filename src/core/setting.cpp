#include "../include/core/setting.hpp"
#include "../include/core/application.hpp"
#include "../include/core/button.hpp"

SettingScene::SettingScene() {}

SettingScene::~SettingScene() {}

void SettingScene::Init() {}

void SettingScene::Update() {
  if (IsKeyPressed(KEY_ESCAPE)) {
    Application::ChangeScene(nullptr);
    return;
  }
}

void SettingScene::Draw() {}
