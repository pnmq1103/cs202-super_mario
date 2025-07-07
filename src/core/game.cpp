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

  if (player_character_) {
    delete player_character_;
    player_character_ = nullptr;
  }
}

void GameScene::Init() {
  App.GetMedia().PlayMusic("ground_theme");

  // Create player character (Mario by default)
  player_character_ = new Character(4.0f); // Scale factor
  player_character_->SetCharacter(
    CharacterType::MARIO, {100.0f, 500.0f}); // Starting position

  // Create command handler and link it to the character
  input_command_ = new Command(player_character_);

  // Load level (stub: replace with actual file path)
  game_manager_.LoadLevel("res/levels/level1.dat");
}

void GameScene::Update() {
  if (IsKeyPressed(KEY_ESCAPE)) {
    App.ChangeScene(nullptr);
    return; // Early return to prevent use-after-free
  }

  // Switch between Mario and Luigi with TAB key
  if (IsKeyPressed(KEY_TAB) && player_character_) {
    if (player_character_->GetCharacter() == CharacterType::MARIO) {
      player_character_->SetCharacter(
        CharacterType::LUIGI, {player_character_->GetRectangle().x,
                               player_character_->GetRectangle().y});
    } else {
      player_character_->SetCharacter(
        CharacterType::MARIO, {player_character_->GetRectangle().x,
                               player_character_->GetRectangle().y});
    }
  }

  // Handle character input through command system
  if (input_command_) {
    input_command_->HandleInput();
  }

  // Update character
  if (player_character_) {
    player_character_->Update();
    player_character_->SetFrameCount();
  }

  // Update game manager (time, lives, etc)
  game_manager_.UpdateTime();
}

void GameScene::Draw() {
  // Draw level (background, blocks, enemies)
  game_manager_.DrawLevel();

  // Draw character
  if (player_character_) {
    player_character_->Draw();
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
