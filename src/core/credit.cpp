#include "include/core/credit.hpp"
#include "include/core/application.hpp"
#include "include/core/constants.hpp"

CreditScene::CreditScene() {}

CreditScene::~CreditScene() {
  UnloadTexture(background_);
}

void CreditScene::Init() {
  background_ = LoadTexture("res/ui/menu_background.png");
}

void CreditScene::Update() {
  if (IsKeyDown(KEY_ESCAPE)) {
    App.RemoveScene();
    return;
  }
}

void CreditScene::Draw() {
  DrawTextureV(
    background_, {0, (constants::screenHeight - background_.height) / 2},
    RAYWHITE);
}

void CreditScene::Resume() {}

SceneType CreditScene::Type() {
  return type_;
}
