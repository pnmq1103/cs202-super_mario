#include "../include/core/credit.hpp"
#include "../include/core/application.hpp"

CreditScene::CreditScene() {}

CreditScene::~CreditScene() {
  UnloadTexture(background_);
}

void CreditScene::Init() {
  background_ = LoadTexture("res/menu_background.png");
}

void CreditScene::Update() {
  if (IsKeyDown(KEY_ESCAPE)) {
    App.ChangeScene(nullptr);
    return;
  }
}

void CreditScene::Draw() {
  timer_       += GetFrameTime();
  float t       = std::min(timer_ / duration_, 1.0f);
  float start_x = (screenWidth - background_.width) / 2;
  float end_x   = 0;
  float ease    = t < 0.5f ? 4 * t * t * t : 1 - powf(-2 * t + 2, 3) / 2;
  float y       = (screenHeight - background_.height) / 2;
  DrawTextureV(background_, {start_x + (end_x - start_x) * ease, y}, RAYWHITE);
}

SceneType CreditScene::Type() {
  return type_;
}
