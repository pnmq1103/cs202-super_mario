#include "../include/core/setting.hpp"
#include "../include/core/application.hpp"
#include "../include/core/button.hpp"

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
    App.ChangeScene(nullptr);
    return;
  }

  UpdateButtons();
  UpdateSliders();
}

void SettingScene::Draw() {
  DrawButtons();
  DrawSliders();
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
    "Music Volume",
    [this]() {
      App.GetMedia().SetMusicVolume(volume_);
    },
    Rectangle{0, 0, 16, 16}, Rectangle{50, 50, 16 * 4, 16 * 4},
    "res/sprites/buttons/volume_on.png");
}

void SettingScene::UpdateSliders() {
  for (size_t i = 0; i < sliders_.size(); ++i) {
    if (i == 0) {
      sliders_[i].Update();
      volume_ = sliders_[i].GetPercentage();
    }
  }
}

void SettingScene::DrawSliders() {
  for (size_t i = 0; i < sliders_.size(); ++i)
    sliders_[i].Draw();
}
