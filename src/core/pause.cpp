#include "include/core/pause.hpp"
#include "include/core/application.hpp"
#include "include/core/constants.hpp"
#include "include/core/setting.hpp"

PauseScene::PauseScene() {
  buttons_.reserve(5);
}

void PauseScene::Init() {
  CreateButtons();
}

void PauseScene::Update() {
  UpdateButtons();
}

void PauseScene::Draw() {
  DrawRectangleRec(
    {0, 0, constants::screenWidth, constants::screenHeight},
    Fade(LIGHTGRAY, 0.9f));

  DrawRectangleRec({box_pos.x, box_pos.y, box_width, box_height}, WHITE);
  DrawRectangleLinesEx({box_pos.x, box_pos.y, box_width, box_height}, 5, RED);

  DrawButtons();
}

void PauseScene::Resume() {}

SceneType PauseScene::Type() {
  return type_;
}

void PauseScene::CreateButtons() {
  float button_width  = 64;
  float button_height = 64;
  float spacing       = 20;

  int button_count = 3;
  if (App.PreviousScene() == SceneType::Game)
    button_count = 5;

  float total_width
    = button_count * button_width + (button_count - 1) * spacing;
  float x = box_pos.x + (box_width - total_width) / 2;
  float y = box_pos.y + (box_height - button_height) / 2;

  Rectangle src = {0, 0, 16, 16};
  Rectangle dst = {x, y, button_width, button_height};

  buttons_.emplace_back(
    "Setting",
    []() {
      App.AddScene(std::make_unique<SettingScene>());
    },
    src, dst, "res/sprites/buttons/setting.png");
  x += button_width + spacing;

  dst = {x, y, button_width, button_height};
  buttons_.emplace_back(
    "Menu",
    []() {
      App.RemoveScene(2);
    },
    src, dst, "res/sprites/buttons/menu.png");
  x += button_width + spacing;

  dst = {x, y, button_width, button_height};
  buttons_.emplace_back(
    "Quit",
    []() {
      App.Close();
    },
    src, dst, "res/sprites/buttons/quit.png");
  x += button_width + spacing;

  dst = {x, y, button_width, button_height};
  if (App.PreviousScene() == SceneType::Game) {
    buttons_.emplace_back(
      "Resume", []() {}, src, dst, "res/sprites/buttons/resume.png");
    x += button_width + spacing;

    dst = {x, y, button_width, button_height};
    buttons_.emplace_back(
      "Restart", []() {}, src, dst, "res/sprites/buttons/restart.png");
  }
}

void PauseScene::UpdateButtons() {
  for (size_t i = 0; i < buttons_.size(); ++i)
    buttons_[i].Update();
}

void PauseScene::DrawButtons() {
  for (size_t i = 0; i < buttons_.size(); ++i)
    buttons_[i].Draw();
}
