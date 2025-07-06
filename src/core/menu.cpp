#include <cmath>
#include <iostream>
#include <memory>
#include <raylib.h>
#include <stdexcept>

#include "../include/core/application.hpp"
#include "../include/core/credit.hpp"
#include "../include/core/editor.hpp"
#include "../include/core/game.hpp"
#include "../include/core/load.hpp"
#include "../include/core/menu.hpp"
#include "../include/core/setting.hpp"

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
  App.GetMedia().PlayMusic("title");
  CreateButtons();
}

void MenuScene::Update() {
  int size = static_cast<int>(menu_items_.size());

  double time = GetTime();
  if (time - last_input_ >= cooldown_) {
    if (IsKeyDown(KEY_DOWN)) {
      last_input_   = time;
      selected_idx_ = (selected_idx_ + 1) % size;
    } else if (IsKeyDown(KEY_UP)) {
      last_input_   = time;
      selected_idx_ = (selected_idx_ - 1 + size) % size;
    } else if (IsKeyDown(KEY_ENTER)) {
      last_input_ = time;

      switch (selected_idx_) {
        case MenuOption::Game:
          App.ChangeScene(std::make_unique<GameScene>());
          break;
        case MenuOption::Load:
          App.ChangeScene(std::make_unique<LoadScene>());
          break;
        case MenuOption::Editor:
          App.ChangeScene(std::make_unique<EditorScene>());
          break;
        case MenuOption::Exit:
          App.Close();
          break;

        default:
          throw std::invalid_argument("Must be positive");
          break;
      }
    }
  }

  UpdateButtons();
}

void MenuScene::Draw() {
  // Draw background
  float x = (screenWidth - background_.width) / 2;
  float y = (screenHeight - background_.height) / 2;
  DrawTextureEx(background_, {x, y}, 0, 1, RAYWHITE);

  // Draw title
  float font_size    = 150;
  const char *title  = "Mario";
  Vector2 title_size = MeasureTextEx(GetFontDefault(), title, font_size, 1);
  x                  = (screenWidth - title_size.x) / 2;
  y                  = (screenHeight - title_size.y) / 6;
  DrawTextEx(GetFontDefault(), title, {x, y}, font_size, 1, RAYWHITE);

  DrawOptions();
  DrawButtons();
}

void MenuScene::DrawOptions() {
  float font_size = 40;
  for (size_t i = 0; i < menu_items_.size(); ++i) {
    const char *option  = menu_items_[i].c_str();
    Vector2 option_size = MeasureTextEx(GetFontDefault(), option, font_size, 1);
    float x             = (screenWidth - option_size.x) / 2;
    float y             = (screenHeight - option_size.y) * (i + 12) / 16;

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
      float space  = 20;
      float length = option_size.y * scale;
      float angle  = static_cast<float>(cos(PI / 6));
      Vector2 a    = {x - space, y + length / 2};
      Vector2 b    = {x - space - length * angle, y};
      Vector2 c    = {x - space - length * angle, y + length};

      DrawTriangle(a, b, c, color);
    }

    DrawTextEx(GetFontDefault(), option, {x, y}, font_size * scale, 3, color);
  }
}

void MenuScene::CreateButtons() {
  // Setting
  buttons_.emplace_back(
    []() {
      App.ChangeScene(std::make_unique<SettingScene>());
    },
    Rectangle{0, 0, 16, 16},
    Rectangle{screenWidth - 48 * 2, screenHeight - 48 * 2, 48, 48},
    "res/sprites/buttons/setting.png");

  // Credit
  buttons_.emplace_back(
    []() {
      App.ChangeScene(std::make_unique<CreditScene>());
    },
    Rectangle{0, 0, 16, 16}, Rectangle{48, screenHeight - 48 * 2, 48, 48},
    "res/sprites/buttons/credit.png");
}

void MenuScene::UpdateButtons() {
  for (size_t i = 0; i < buttons_.size(); ++i)
    buttons_[i].Update();
}

void MenuScene::DrawButtons() {
  for (size_t i = 0; i < buttons_.size(); ++i)
    buttons_[i].Draw();
}
