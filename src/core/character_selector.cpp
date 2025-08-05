#include <cmath>
#include <iostream>
#include <memory>
#include <raylib.h>

#include "include/core/application.hpp"
#include "include/core/character_selector.hpp"
#include "include/core/constants.hpp"
#include "include/core/game.hpp"

// Static member definition
SelectedCharacter CharacterSelectorScene::global_selected_character_
  = SelectedCharacter::MARIO;

CharacterSelectorScene::CharacterSelectorScene() {}

CharacterSelectorScene::~CharacterSelectorScene() {
  UnloadTexture(background_);
}

void CharacterSelectorScene::Init() {
  background_ = LoadTexture("res/ui/character_selection.png");
  App.Media().PlayMusic("choose_character");

  selectedCharacter_         = SelectedCharacter::MARIO;
  global_selected_character_ = SelectedCharacter::MARIO;

  last_input_ = GetTime();
}

void CharacterSelectorScene::Update() {
  double time = GetTime();

  double time_since_last_input = time - last_input_;
  if (time_since_last_input >= cooldown_) {

    if (IsKeyPressed(KEY_RIGHT)) {
      last_input_ = time;
      SelectMario();
    } else if (IsKeyPressed(KEY_LEFT)) {
      last_input_ = time;
      SelectLuigi();
    }
    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
      last_input_ = time;
      std::cout << "Start game key pressed!" << std::endl;
      StartGame();
    }

    if (IsKeyPressed(KEY_ESCAPE)) {
      last_input_ = time;
      std::cout << "Escape pressed, returning to menu" << std::endl;
      App.RemoveScene();
    }
  }
}

void CharacterSelectorScene::Draw() {
  float x = (constants::screenWidth - background_.width) / 2;
  float y = (constants::screenHeight - background_.height) / 2;

  DrawTextureV(background_, {x, y}, RAYWHITE);

  float font_sz     = 90;
  const char *title = "Choose Your Character";
  Vector2 title_sz  = MeasureTextEx(GetFontDefault(), title, font_sz, 1);
  float title_x     = (constants::screenWidth - title_sz.x) / 2;
  float title_y     = constants::screenHeight * 0.15f;

  DrawTextEx(GetFontDefault(), title, {title_x, title_y}, font_sz, 1, DARKBLUE);
  DrawCharacterOptions();
  DrawInstructions();
}

void CharacterSelectorScene::Resume() {}

SceneType CharacterSelectorScene::Type() {
  return type_;
}

void CharacterSelectorScene::SelectMario() {
  selectedCharacter_         = SelectedCharacter::MARIO;
  global_selected_character_ = SelectedCharacter::MARIO;
  std::cout << "Mario selected!" << std::endl;
}

void CharacterSelectorScene::SelectLuigi() {
  selectedCharacter_         = SelectedCharacter::LUIGI;
  global_selected_character_ = SelectedCharacter::LUIGI;
  std::cout << "Luigi selected!" << std::endl;
}

void CharacterSelectorScene::StartGame() {
  std::cout << "Starting game with "
            << (selectedCharacter_ == SelectedCharacter::MARIO ? "Mario"
                                                               : "Luigi")
            << std::endl;
  App.AddScene(std::make_unique<GameScene>());
}

void CharacterSelectorScene::DrawCharacterOptions() {
  float centerX = constants::screenWidth / 2.0f;
  float centerY = constants::screenHeight / 2.0f;

  // Character option positions
  float mario_x = centerX + 200.0f;
  float luigi_x = centerX - 200.0f;
  float char_y  = centerY;

  // Character sizes
  float char_width  = 150.0f;
  float char_height = 200.0f;

  // Draw Mario option
  Color mario_color
    = (selectedCharacter_ == SelectedCharacter::MARIO) ? YELLOW : WHITE;
  float mario_scale
    = (selectedCharacter_ == SelectedCharacter::MARIO) ? 1.2f : 1.0f;

  // Draw Mario rectangle (placeholder for character sprite)
  Rectangle mario_rect = {
    mario_x - char_width * mario_scale / 2,
    char_y - char_height * mario_scale / 2, char_width * mario_scale,
    char_height * mario_scale};
  DrawRectangleRec(mario_rect, ColorAlpha(RED, 0.7f));
  DrawRectangleLinesEx(mario_rect, 5, mario_color);

  const char *mario_label = "MARIO";
  Vector2 mario_label_sz  = MeasureTextEx(GetFontDefault(), mario_label, 30, 1);
  DrawTextEx(
    GetFontDefault(), mario_label,
    {mario_x - mario_label_sz.x / 2,
     char_y + char_height * mario_scale / 2 + 20},
    30, 1, mario_color);

  Color luigi_color
    = (selectedCharacter_ == SelectedCharacter::LUIGI) ? YELLOW : WHITE;
  float luigi_scale
    = (selectedCharacter_ == SelectedCharacter::LUIGI) ? 1.2f : 1.0f;

  Rectangle luigi_rect = {
    luigi_x - char_width * luigi_scale / 2,
    char_y - char_height * luigi_scale / 2, char_width * luigi_scale,
    char_height * luigi_scale};
  DrawRectangleRec(luigi_rect, ColorAlpha(GREEN, 0.7f));
  DrawRectangleLinesEx(luigi_rect, 5, luigi_color);

  const char *luigi_label = "LUIGI";
  Vector2 luigi_label_sz  = MeasureTextEx(GetFontDefault(), luigi_label, 30, 1);
  DrawTextEx(
    GetFontDefault(), luigi_label,
    {luigi_x - luigi_label_sz.x / 2,
     char_y + char_height * luigi_scale / 2 + 20},
    30, 1, luigi_color);

  double time       = GetTime();
  float wave        = static_cast<float>((sin(time * 4) + 1) * 0.5);
  float indicator_y = char_y - char_height / 2 - 50;

  if (selectedCharacter_ == SelectedCharacter::MARIO) {
    float alpha = 0.5f + 0.5f * wave;
    DrawText(
      "", (int)(mario_x - 10), (int)indicator_y, 40, ColorAlpha(YELLOW, alpha));
  } else {
    float alpha = 0.5f + 0.5f * wave;
    DrawText(
      "", (int)(luigi_x - 10), (int)indicator_y, 40, ColorAlpha(YELLOW, alpha));
  }
}

void CharacterSelectorScene::DrawInstructions() {
  float instructions_y = constants::screenHeight * 0.8f;

  const char *instructions[] = {
    "Use LEFT/RIGHT arrows to select character",
    "Press ENTER or SPACE to start game", "Press ESC to return to menu"};

  for (int i = 0; i < 3; i++) {
    Vector2 text_sz = MeasureTextEx(GetFontDefault(), instructions[i], 20, 1);
    float text_x    = (constants::screenWidth - text_sz.x) / 2;
    DrawTextEx(
      GetFontDefault(), instructions[i], {text_x, instructions_y + i * 30}, 20,
      1, DARKGRAY);
  }
}
