#include "include/core/setting.hpp"
#include "include/core/application.hpp"
#include "include/core/button.hpp"
#include "include/core/constants.hpp"
#include "include/core/file_handler.hpp"

SettingScene::SettingScene() {
  buttons_.reserve(3);
  sliders_.reserve(2);
}

SettingScene::~SettingScene() {}

void SettingScene::Init() {
  CreateButtons();
  CreateSliders();
}

void SettingScene::Update() {
  if (IsKeyPressed(KEY_ESCAPE)) {
    App.RemoveScene();
    return;
  }

  UpdateButtons();
  UpdateSliders();
}

void SettingScene::Draw() {
  DrawRectangleRec(
    {0, 0, constants::screenWidth, constants::screenHeight}, WHITE);
  DrawButtons();
  DrawSliders();
}

void SettingScene::Resume() {}

SceneType SettingScene::Type() {
  return type_;
}

void SettingScene::CreateButtons() {}

void SettingScene::UpdateButtons() {
  for (size_t i = 0; i < buttons_.size(); ++i)
    buttons_[i].Update();
}

void SettingScene::DrawButtons() {
  for (size_t i = 0; i < buttons_.size(); ++i)
    buttons_[i].Draw();
}

void SettingScene::CreateSliders() {
  sliders_.emplace_back(
    "Music", []() {}, Rectangle{0, 0, 16, 16},
    Rectangle{50, 50, 16 * 4, 16 * 4}, "res/ui/buttons/volume_on.png",
    App.Config().GetMusicVolume());
}

void SettingScene::UpdateSliders() {
  for (size_t i = 0; i < sliders_.size(); ++i) {
    sliders_[i].Update();
    if (i == 0) {
      float volume = sliders_[i].GetPercentage();
      App.Config().SetMusicVolume(volume);
      App.Media().SetMusicVolume(volume);
    }
  }
}

void SettingScene::DrawSliders() {
  for (size_t i = 0; i < sliders_.size(); ++i)
    sliders_[i].Draw();
}
