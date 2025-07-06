#include "../include/core/credit.hpp"
#include "../include/core/application.hpp"

CreditScene::CreditScene() {}

CreditScene::~CreditScene() {}

void CreditScene::Init() {}

void CreditScene::Update() {
  if (IsKeyDown(KEY_ESCAPE)) {
    Application::ChangeScene(nullptr);
    return;
  }
}

void CreditScene::Draw() {}
