#include "../include/core/setting.hpp"
#include "../include/core/application.hpp"
#include "../include/core/button.hpp"

SettingScene::SettingScene() {
  buttons_.reserve(3);
  sliders_.reserve(2);
}

SettingScene::~SettingScene() {
  UnloadTexture(background_);
}

void SettingScene::Init() {
  background_ = LoadTexture("res/menu_background.png");
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
  timer_       += GetFrameTime();
  float t       = std::min(timer_ / duration_, 1.0f);
  float start_x = (screenWidth - background_.width) / 2;
  float end_x   = screenWidth - background_.width;
  float ease    = t < 0.5f ? 4 * t * t * t : 1 - powf(-2 * t + 2, 3) / 2;
  float y       = (screenHeight - background_.height) / 2;
  DrawTextureV(background_, {start_x + (end_x - start_x) * ease, y}, RAYWHITE);

  DrawButtons();
  DrawSliders();
}

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
