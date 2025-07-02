#include <raylib.h>

#include "include/application.hpp"
#include "include/character.hpp"
#include "include/command.hpp"
#include "include/game.hpp"

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
  Application::GetInstance().GetMedia().PlayMusic("ground_theme");

  // Create player character (Mario by default)
  player_character_ = new Character(4.0f); // Scale factor
  player_character_->SetCharacter(
    CharacterType::MARIO, {100.0f, 500.0f}); // Starting position

  // Create command handler and link it to the character
  input_command_ = new Command(player_character_);
}

void GameScene::Update() {
  if (IsKeyPressed(KEY_ESCAPE)) {
    Application::ChangeScene(nullptr);
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
}

void GameScene::Draw() {
  // Draw character
  if (player_character_) {
    player_character_->Draw();
  }

  // Draw instructions
  DrawText("Controls:", 10, 30, 20, DARKBLUE);
  DrawText("Arrow Keys/WASD: Move", 10, 55, 20, DARKBLUE);
  DrawText("Space/Up/W: Jump", 10, 80, 20, DARKBLUE);
  DrawText("X/Ctrl: Fireball (Fire mode only)", 10, 105, 20, DARKBLUE);
  DrawText("TAB: Switch Mario/Luigi", 10, 130, 20, DARKBLUE);
  DrawText("ESC: Back to menu", 10, 155, 20, DARKBLUE);
}
