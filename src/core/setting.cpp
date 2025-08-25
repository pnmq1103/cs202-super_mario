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

  const char *title = "Setting";
  DrawText(title, 1024 / 2 - MeasureText(title, 80) / 2, 40, 80, BLACK);

  DrawButtons();
  DrawSliders();
}

void SettingScene::Resume() {}

SceneType SettingScene::Type() {
  return type_;
}

void SettingScene::CreateButtons() {
  Rectangle src = {0, 0, 16, 16};
  Rectangle dst = {32, 32, 64, 64};
  buttons_.emplace_back(
    "Return",
    []() {
      App.RemoveScene();
    },
    src, dst, "res/ui/buttons/return.png");
}

void SettingScene::UpdateButtons() {
  for (auto &button : buttons_)
    button.Update();
}

void SettingScene::DrawButtons() {
  for (auto &button : buttons_)
    button.Draw();
}

void SettingScene::CreateSliders() {
  float x = 64;
  float y = 64 * 3;
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
  for (auto &slider : sliders_)
    slider.Draw();
}
