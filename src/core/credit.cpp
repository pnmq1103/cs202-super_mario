#include "include/core/credit.hpp"
#include "include/core/application.hpp"
#include "include/core/constants.hpp"

CreditScene::CreditScene() {
  buttons_.reserve(3);
}

CreditScene::~CreditScene() {}

void CreditScene::Init() {
  Rectangle src = {0, 0, 16, 16};
  Rectangle dst = {32, 32, 64, 64};
  buttons_.emplace_back(
    "Return",
    []() {
      App.RemoveScene();
    },
    src, dst, "res/ui/buttons/return.png");
}

void CreditScene::Update() {
  if (IsKeyDown(KEY_ESCAPE)) {
    App.RemoveScene();
    return;
  }

  for (auto &button : buttons_)
    button.Update();
}

void CreditScene::Draw() {
  DrawRectangleRec(
    {0, 0, constants::screenWidth, constants::screenHeight}, WHITE);

  const char *title = "Credit";
  DrawText(title, 1024 / 2 - MeasureText(title, 80) / 2, 40, 80, BLACK);

  for (auto &button : buttons_)
    button.Draw();
}

void CreditScene::Resume() {}

SceneType CreditScene::Type() {
  return type_;
}
