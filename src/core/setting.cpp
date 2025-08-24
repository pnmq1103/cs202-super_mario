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
  float x = 64;
  float y = 64 * 2;
  sliders_.emplace_back(
    "Music", []() {}, Rectangle{0, 0, 16, 16}, Rectangle{x, y, 16 * 4, 16 * 4},
    "res/ui/buttons/volume_on.png", App.Config().GetMusicVolume());

  x = constants::screenWidth - (64 + 16 + 64 * 4 + 64);
  sliders_.emplace_back(
    "Sound", []() {}, Rectangle{0, 0, 16, 16}, Rectangle{x, y, 16 * 4, 16 * 4},
    "res/ui/buttons/volume_on.png", App.Config().GetSoundVolume());
}

void SettingScene::UpdateSliders() {
  for (size_t i = 0; i < sliders_.size(); ++i) {
    sliders_[i].Update();
    if (i == 0) {
      float volume = sliders_[i].GetPercentage();
      App.Config().SetMusicVolume(volume);
      App.Media().SetMusicVolume(volume);
    } else if (i == 1) {
      float sound = sliders_[i].GetPercentage();
      App.Config().SetSoundVolume(sound);
      App.Media().SetSoundVolume(sound);
    }
  }
}

void SettingScene::DrawSliders() {
  for (size_t i = 0; i < sliders_.size(); ++i)
    sliders_[i].Draw();
}
