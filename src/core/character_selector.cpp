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
  CreateRegions();
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
    if (IsKeyPressed(KEY_ENTER)) {
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

  Vector2 mouse = GetMousePosition();
  if (
    CheckCollisionPointRec(mouse, left_rec)
    || CheckCollisionPointTriangle(mouse, left_tri.a, left_tri.b, left_tri.c)) {
    hover_left = true;
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
      SelectLuigi();
      StartGame();
    }
  } else {
    hover_left = false;
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
      SelectMario();
      StartGame();
    }
  }
}

void CharacterSelectorScene::Draw() {
  DrawTexture(background_, 0, 0, RAYWHITE);

  float font_sz     = 25;
  const char *title = "Choose Your Character";
  Vector2 title_sz  = MeasureTextEx(GetFontDefault(), title, font_sz, 1);
  float title_x     = (constants::screenWidth - title_sz.x) / 2;
  float title_y     = constants::screenHeight / 2;
  DrawTextEx(GetFontDefault(), title, {title_x, title_y}, font_sz, 1, WHITE);

  DrawInstructions();

  Color dark   = ColorAlpha(BLACK, 0.3f);
  Color bright = ColorAlpha(BLACK, 0);
  DrawRectangleRec(left_rec, hover_left ? bright : dark);
  DrawTriangle(left_tri.a, left_tri.b, left_tri.c, hover_left ? bright : dark);

  DrawRectangleRec(right_rec, hover_left ? dark : bright);
  DrawTriangle(
    right_tri.a, right_tri.b, right_tri.c, hover_left ? dark : bright);
}

void CharacterSelectorScene::Resume() {}

SceneType CharacterSelectorScene::Type() {
  return type_;
}

void CharacterSelectorScene::CreateRegions() {
  // Dividing line
  Vector2 p1 = {566, 0};
  Vector2 p2 = {430, 960};
  float d    = p1.x - p2.x;

  left_rec  = {0, 0, p2.x, constants::screenHeight};
  right_rec = {p1.x, 0, constants::screenWidth - p2.x, constants::screenHeight};

  left_tri  = {p1, {p1.x - d, p1.y}, p2};
  right_tri = {p1, p2, {p1.x, p2.y}};
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

void CharacterSelectorScene::DrawInstructions() {
  float instructions_y = constants::screenHeight * 0.8f;

  const char *instructions[] = {
    "LEFT/ RIGHT arrows to select character", "ENTER/ SPACE to start game",
    "ESC to return"};

  for (int i = 0; i < 3; i++) {
    Vector2 text_sz = MeasureTextEx(GetFontDefault(), instructions[i], 20, 1);
    float text_x    = (constants::screenWidth - text_sz.x) / 2;
    DrawTextEx(
      GetFontDefault(), instructions[i], {text_x, instructions_y + i * 30}, 20,
      1, WHITE);
  }
}
