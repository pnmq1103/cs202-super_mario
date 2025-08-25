#include "include/core/credit.hpp"
#include "include/core/application.hpp"
#include "include/core/constants.hpp"

CreditScene::CreditScene() {}

CreditScene::~CreditScene() {}

void CreditScene::Init() {}

void CreditScene::Update() {
  if (IsKeyDown(KEY_ESCAPE)) {
    App.RemoveScene();
    return;
  }
}

void CreditScene::Draw() {
  DrawRectangleRec(
    {0, 0, constants::screenWidth, constants::screenHeight}, WHITE);

  const char *title = "Credit";
  DrawText(title, 1024 / 2 - MeasureText(title, 80) / 2, 40, 80, BLACK);
}

void CreditScene::Resume() {}

SceneType CreditScene::Type() {
  return type_;
}
