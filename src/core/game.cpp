#include <raylib.h>
#include <stdexcept>

#include "include/characters/character.hpp"
#include "include/core/application.hpp"
#include "include/core/command.hpp"
#include "include/core/game.hpp"
#include "include/core/game_managing.hpp"

GameScene::GameScene() : game_manager_() {}

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
  App.Media().PlayMusic("ground_theme");

  // Create Mario character
  mario_character_ = new Character(4.0f);
  mario_character_->SetCharacter(CharacterType::MARIO, {100.0f, 500.0f});

  // Create Luigi character
  luigi_character_ = new Character(4.0f);
  luigi_character_->SetCharacter(CharacterType::LUIGI, {100.0f, 500.0f});

  // Create command handler with both characters (Mario starts as active)
  input_command_ = new Command(mario_character_, luigi_character_);

  // Initialize collision system in game manager with proper world size
  game_manager_.InitializeCollisionSystem(3200.0f, 1000.0f);

  // Register characters with collision system
  game_manager_.RegisterCharacterWithCollision(mario_character_);
  game_manager_.RegisterCharacterWithCollision(luigi_character_);

  // Load level
  game_manager_.LoadLevel("res/saves/map_00.json");
}

void GameScene::Update() {
  if (IsKeyPressed(KEY_ESCAPE)) {
    App.RemoveScene();
    return;
  }

  // Handle character input through command system (includes instructions
  // handling)
  if (input_command_) {
    input_command_->HandleInput();
  }

  // Get active character
  Character *active_character = input_command_->GetActiveCharacter();

  // Update character frame count and physics
  if (active_character) {
    active_character->SetFrameCount();
    active_character->Update();
  }

  // Update game manager with enemies and collision system
  float dt = GetFrameTime();
  game_manager_.Update(dt, active_character);

  // Check for game over or level complete
  if (game_manager_.IsGameOver()) {
    // Handle game over (could transition to game over scene)
    if (IsKeyPressed(KEY_R)) {
      game_manager_.ResetGame();
      game_manager_.LoadLevel("res/saves/map_00.json");
    }
  }

  if (game_manager_.IsLevelComplete()) {
    // Handle level completion
    if (IsKeyPressed(KEY_ENTER)) {
      game_manager_.LoadNextLevel();
    }
  }
}

void GameScene::Draw() {
  // Draw level (background, blocks, enemies) - this uses the camera
  game_manager_.DrawLevel();

  // Draw active character - this is handled inside DrawLevel with camera
  Character *active_character = input_command_->GetActiveCharacter();
  if (active_character) {
    // Character drawing is handled by the camera system in DrawLevel
    // We draw character here with camera
    BeginMode2D(game_manager_.GetCamera());
    active_character->Draw();
    EndMode2D();
  }

  // Draw instructions button (from Command system)
  if (input_command_) {
    input_command_->DrawInstructionsButton();
  }

  // Show instructions panel if active (from Command system)
  if (input_command_ && input_command_->IsShowingInstructions()) {
    input_command_->DrawInstructionsPanel();
  } else {
    // Draw minimal UI when instructions are hidden
    // Character status
    if (active_character) {
      const char *charName
        = (active_character == mario_character_) ? "Mario" : "Luigi";
      const char *stateNames[] = {"Small", "Big", "Fire/Electric"};

      DrawText(
        TextFormat("Active: %s", charName), GetScreenWidth() - 200, 200, 16,
        YELLOW);
      DrawText(
        TextFormat("State: %s", stateNames[active_character->GetState()]),
        GetScreenWidth() - 200, 220, 14, WHITE);

      if (active_character->IsStarman()) {
        DrawText("STAR POWER!", GetScreenWidth() - 200, 240, 14, GOLD);
      }
    }

    // Compact controls reminder
    DrawText("Press 'I' or click Help for instructions", 10, 270, 14, DARKGRAY);
  }

  // Game over/level complete overlays
  if (game_manager_.IsGameOver()) {
    DrawText(
      "GAME OVER", GetScreenWidth() / 2 - 80, GetScreenHeight() / 2 - 40, 28,
      RED);
    DrawText(
      "Press R to restart or ESC to exit", GetScreenWidth() / 2 - 150,
      GetScreenHeight() / 2, 16, WHITE);
  }

  if (game_manager_.IsLevelComplete()) {
    DrawText(
      "LEVEL COMPLETE!", GetScreenWidth() / 2 - 100, GetScreenHeight() / 2 - 40,
      24, GOLD);
    DrawText(
      "Press ENTER for next level", GetScreenWidth() / 2 - 120,
      GetScreenHeight() / 2, 16, WHITE);
  }
}

void GameScene::Resume() {
  // Resume any paused systems
  game_manager_.ResumeEnemies();

  // Resume background music if it was paused
  App.Media().PlayMusic("ground_theme");
}

SceneType GameScene::Type() {
  return type_;
}
