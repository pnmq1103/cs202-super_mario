#include "include/setting.hpp"
#include "include/application.hpp"
#include "include/button.hpp"

SettingScene::SettingScene() {}

SettingScene::~SettingScene() {}

void SettingScene::Init() {}

void SettingScene::Update() {
  if (IsKeyPressed(KEY_ESCAPE))
    Application::ChangeScene(nullptr);
}

void SettingScene::Draw() {}
