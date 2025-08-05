#include <cmath>
#include <iostream>
#include <memory>
#include <raylib.h>
#include <stdexcept>

#include "include/core/application.hpp"
#include "include/core/constants.hpp"
#include "include/core/credit.hpp"
#include "include/core/editor.hpp"
#include "include/core/game.hpp"
#include "include/core/load.hpp"
#include "include/core/menu.hpp"
#include "include/core/setting.hpp"

MenuScene::MenuScene() {
  menu_items_.resize(4);
  menu_items_ = {"Play", "Load", "Editor", "Exit"};
  // This is important since we don't have copy constructor for Button
  buttons_.reserve(2);
}

MenuScene::~MenuScene() {
  UnloadTexture(background_);
}

void MenuScene::Init() {
  background_ = LoadTexture("res/menu_background.png");
  App.Media().PlayMusic("title");
  CreateButtons();
}

void MenuScene::Update() {
  int sz = static_cast<int>(menu_items_.size());

  double time = GetTime();
  if (time - last_input_ >= cooldown_) {
    if (IsKeyDown(KEY_DOWN)) {
      last_input_   = time;
      selected_idx_ = (selected_idx_ + 1) % sz;
    } else if (IsKeyDown(KEY_UP)) {
      last_input_   = time;
      selected_idx_ = (selected_idx_ - 1 + sz) % sz;
    } else if (IsKeyDown(KEY_ENTER)) {
      last_input_ = time;

      switch (selected_idx_) {
        case SceneType::Game:
          App.AddScene(std::make_unique<GameScene>());
          break;
        case SceneType::Load:
          App.AddScene(std::make_unique<LoadScene>());
          break;
        case SceneType::Editor:
          App.AddScene(std::make_unique<EditorScene>());
          break;
        case SceneType::Exit:
          App.Close();
          break;

        default:
          throw std::invalid_argument("Must be positive");
      }
    }
  }

  UpdateButtons();
}

void MenuScene::Draw() {
  // Draw background
  timer_     += GetFrameTime();
  float t     = std::min(timer_ / duration_, 1.0f);
  float end_x = (constants::screenWidth - background_.width) / 2;

  float start_x = end_x;
  if (App.PreviousScene() == SceneType::Setting)
    start_x = constants::screenWidth - background_.width;
  else if (App.PreviousScene() == SceneType::Credit)
    start_x = 0;

  float ease  = 1 - powf(1 - t, 3);
  float end_y = (constants::screenHeight - background_.height) / 2;

  DrawTextureV(
    background_, {start_x + (end_x - start_x) * ease, end_y}, RAYWHITE);

  // Draw title
  float font_sz     = 150;
  const char *title = "Mario";
  Vector2 title_sz  = MeasureTextEx(GetFontDefault(), title, font_sz, 1);
  float x           = (constants::screenWidth - title_sz.x) / 2;
  float y           = (constants::screenHeight - title_sz.y) / 6;
  DrawTextEx(GetFontDefault(), title, {x, y}, font_sz, 1, RAYWHITE);

  DrawOptions();
  DrawButtons();
}

void MenuScene::Resume() {
  timer_ = 0;
}

SceneType MenuScene::Type() {
  return type_;
}

void MenuScene::DrawOptions() {
  float font_sz = 40;
  for (size_t i = 0; i < menu_items_.size(); ++i) {
    const char *option  = menu_items_[i].c_str();
    Vector2 option_size = MeasureTextEx(GetFontDefault(), option, font_sz, 1);
    float x             = (constants::screenWidth - option_size.x) / 2;
    float y = (constants::screenHeight - option_size.y) * (i + 12) / 16;

    double time   = GetTime();
    float wave    = static_cast<float>((sin(time * 3) + 1) * 0.5); // [0, 1]
    float pulsing = 0.1f * wave;
    float scale   = 1;
    float opacity = 0.5f + 0.5f * wave;
    Color color   = RAYWHITE;

    if (i == selected_idx_) {
      scale = pulsing + 1;
      color = {245, 245, 245, static_cast<unsigned char>(255 * opacity)};

      // Draw arrow
      float spacing = 20;
      float length  = option_size.y * scale;
      Vector2 a     = {x - spacing, y + length / 2};
      Vector2 b     = {x - spacing - length * cosf(PI / 6), y};
      Vector2 c     = {x - spacing - length * cosf(PI / 6), y + length};

      DrawTriangle(a, b, c, color);
    }

    DrawTextEx(GetFontDefault(), option, {x, y}, font_sz * scale, 3, color);
  }
}

void MenuScene::CreateButtons() {
  // Setting
  buttons_.emplace_back(
    "Setting",
    []() {
      App.AddScene(std::make_unique<SettingScene>());
    },
    Rectangle{0, 0, 16, 16},
    Rectangle{
      constants::screenWidth - 64 * 2, constants::screenHeight - 64 * 2, 64,
      64},
    "res/sprites/buttons/setting.png");

  // Credit
  buttons_.emplace_back(
    "Credit",
    []() {
      App.AddScene(std::make_unique<CreditScene>());
    },
    Rectangle{0, 0, 16, 16},
    Rectangle{64, constants::screenHeight - 64 * 2, 64, 64},
    "res/sprites/buttons/option.png");
}

void MenuScene::UpdateButtons() {
  for (size_t i = 0; i < buttons_.size(); ++i)
    buttons_[i].Update();
}

void MenuScene::DrawButtons() {
  for (size_t i = 0; i < buttons_.size(); ++i)
    buttons_[i].Draw();
}
