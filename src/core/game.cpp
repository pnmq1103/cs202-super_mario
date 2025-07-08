#include <raylib.h>

#include "../include/characters/character.hpp"
#include "../include/core/application.hpp"
#include "../include/core/command.hpp"
#include "../include/core/game.hpp"
#include "../include/core/game_managing.hpp"

GameScene::GameScene() {}

GameScene::~GameScene() {
  // Set pointers to nullptr first to prevent further use
  if (input_command_) {
    input_command_->SetActiveCharacter(nullptr);
    delete input_command_;
    input_command_ = nullptr;
  }

  if (mario_character_) {
    delete mario_character_;
    mario_character_ = nullptr;
  }

  if (luigi_character_) {
    delete luigi_character_;
    luigi_character_ = nullptr;
  }
}

void GameScene::Init() {
  App.GetMedia().PlayMusic("ground_theme");

  // Create Mario character
  mario_character_ = new Character(4.0f);
  mario_character_->SetCharacter(CharacterType::MARIO, {100.0f, 500.0f});

  // Create Luigi character
  luigi_character_ = new Character(4.0f);
  luigi_character_->SetCharacter(CharacterType::LUIGI, {100.0f, 500.0f});

  // Create command handler with both characters (Mario starts as active)
  input_command_ = new Command(mario_character_, luigi_character_);

  // Load level
  game_manager_.LoadLevel("res/levels/level1.dat");
}

void GameScene::Update() {
  if (IsKeyPressed(KEY_ESCAPE)) {
    App.ChangeScene(nullptr);
    return;
  }

  // Handle character input through command system (includes TAB switching)
  if (input_command_) {
    input_command_->HandleInput();
  }

  // Update active character
  Character *active_character = input_command_->GetActiveCharacter();
  if (active_character) {
    active_character->Update();
    active_character->SetFrameCount();
  }

  // Update game manager (time, lives, etc)
  game_manager_.UpdateTime();
}

void GameScene::Draw() {
  // Draw level (background, blocks, enemies)
  game_manager_.DrawLevel();

  // Draw active character
  Character *active_character = input_command_->GetActiveCharacter();
  if (active_character) {
    active_character->Draw();
  }

  // Draw stats (lives, points, time)
  game_manager_.DrawStats();

  int controlsStartY = 110;
  int lineSpacing    = 25;

  DrawText("Controls:", 10, controlsStartY + lineSpacing * 0, 20, DARKBLUE);
  DrawText(
    "Arrow Keys/WASD: Move", 10, controlsStartY + lineSpacing * 1, 20,
    DARKBLUE);
  DrawText(
    "Space/Up/W: Jump", 10, controlsStartY + lineSpacing * 2, 20, DARKBLUE);
  DrawText(
    "X/Ctrl: Fireball (Fire mode only)", 10, controlsStartY + lineSpacing * 3,
    20, DARKBLUE);
  DrawText(
    "TAB: Switch Mario/Luigi", 10, controlsStartY + lineSpacing * 4, 20,
    DARKBLUE);
  DrawText(
    "ESC: Back to menu", 10, controlsStartY + lineSpacing * 5, 20, DARKBLUE);
}

SceneType GameScene::Type() {
  return type_;
}
