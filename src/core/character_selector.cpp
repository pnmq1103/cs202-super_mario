#include <cmath>
#include <iostream>
#include <memory>
#include <raylib.h>

#include "include/core/application.hpp"
#include "include/core/character_selector.hpp"
#include "include/core/constants.hpp"
#include "include/core/game.hpp"

CharacterType CharacterSelectorScene::selected_character_
  = CharacterType::LUIGI;

CharacterSelectorScene::CharacterSelectorScene(std::string level) : levelPath(level) {}

CharacterSelectorScene::~CharacterSelectorScene() {
  UnloadTexture(background_);
  UnloadTexture(mario_);
  UnloadTexture(luigi_);
}

void CharacterSelectorScene::Init() {
  background_ = LoadTexture("res/ui/character_selection.png");
  mario_      = LoadTexture("res/ui/character_selection_mario.png");
  luigi_      = LoadTexture("res/ui/character_selection_luigi.png");
  App.Media().PlayMusic("choose_character");

  selected_character_ = CharacterType::LUIGI;
}

void CharacterSelectorScene::Update() {
  if (IsKeyPressed(KEY_ESCAPE)) {
    App.RemoveScene();
    return;
  }
  double time = GetTime();

  double last_input = time - last_input_;
  if (last_input >= cooldown_) {
    if (IsKeyPressed(KEY_LEFT)) {
      last_input_         = time;
      hover_left          = true;
      selected_character_ = CharacterType::LUIGI;
    } else if (IsKeyPressed(KEY_RIGHT)) {
      last_input_         = time;
      hover_left          = false;
      selected_character_ = CharacterType::MARIO;
    } else if (IsKeyPressed(KEY_ENTER)) {
      last_input_ = time;
      std::cout << "Start game\n";
      StartGame();
    }
  }

  // Vector2 mouse = GetMousePosition();
  // if (
  //   CheckCollisionPointRec(mouse, left_rec)
  //   || CheckCollisionPointTriangle(mouse, left_tri.a, left_tri.b,
  //   left_tri.c)) { hover_left = true; if
  //   (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
  //     SelectLuigi();
  //     StartGame();
  //   }
  // } else {
  //   hover_left = false;
  //   if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
  //     SelectMario();
  //     StartGame();
  //   }
  // }
}

void CharacterSelectorScene::Draw() {
  DrawTexture(background_, 0, 0, RAYWHITE);

  Color dark   = ColorAlpha(GRAY, 0.3f);
  Color bright = ColorAlpha(WHITE, 1);
  DrawTexture(luigi_, 0, 0, hover_left ? bright : dark);
  DrawTexture(mario_, 0, 0, hover_left ? dark : bright);

  DisplayPower();
}

void CharacterSelectorScene::Resume() {}

SceneType CharacterSelectorScene::Type() {
  return type_;
}

void CharacterSelectorScene::StartGame() {
  std::cout << "Starting game with "
            << (selected_character_ == CharacterType::MARIO ? "Mario" : "Luigi")
            << '\n';
  App.AddScene(std::make_unique<GameScene>(selected_character_, levelPath));
  
}

CharacterType CharacterSelectorScene::GetCharacterType() {
  return selected_character_;
}

void CharacterSelectorScene::DisplayPower() {
  const char *mario[] = {"SPEED: 10", "JUMP: 3", "FIRE"};
  const char *luigi[] = {"SPEED: 7", "JUMP: 5", "ELECTRIC"};

  float y = constants::screenHeight * 0.45;
  if (hover_left) {
    for (int i = 0; i < 3; i++) {
      float x = 380;
      DrawTextEx(GetFontDefault(), luigi[i], {x, y + i * 30}, 20, 1, WHITE);
    }
  } else {
    for (int i = 0; i < 3; i++) {
      float x = 550;
      DrawTextEx(GetFontDefault(), mario[i], {x, y + i * 30}, 20, 1, WHITE);
    }
  }
}