#include <raylib.h>
#include <raymath.h>
#include <stdexcept>

#include "include/characters/character.hpp"
#include "include/core/application.hpp"
#include "include/core/character_selector.hpp"
#include "include/core/command.hpp"
#include "include/core/constants.hpp"
#include "include/core/file_handler.hpp"
#include "include/core/game.hpp"
#include "include/core/game_info.hpp"
#include "include/core/game_managing.hpp"
#include "include/core/pause.hpp"
#include "include/managers/enemy_manager.hpp"
#include "include/objects/object_manager.hpp"

CollisionHandler GameScene::collision_handler_(
  constants::mapWidth * 16 * constants::scale,
  constants::mapHeight * 16 * constants::scale);

GameScene::GameScene(CharacterType type, int level)
    : game_manager_(), character_type_(type), current_level_(level) {

  is_level_loaded_ = false;
}

GameScene::GameScene(CharacterType type, std::string level)
    : game_manager_(), character_type_(type), current_level_(1) {
  if (level == "res/maps/map2.json")
    current_level_ = 2;
  else if (level == "res/maps/map3.json")
    current_level_ = 3;
  GameInfo::GetInstance().Reset();
  is_level_loaded_ = true;
  levelPath        = level;
};

GameScene::~GameScene() {
  EnemyManager::GetInstance().ClearAllEnemies();

  if (input_command_) {
    delete input_command_;
    input_command_ = nullptr;
  }

  if (player_character_) {
    delete player_character_;
    player_character_ = nullptr;
  }

  App.ToggleCustomCursor();
}

void GameScene::Init() {
  App.ToggleCustomCursor();

  camera_.target   = {0, 0};
  camera_.offset   = {constants::screenWidth / 2, constants::screenHeight / 2};
  camera_.rotation = 0;
  camera_.zoom     = 1;

  collision_handler_.Reset(
    constants::mapWidth * 16 * constants::scale,
    constants::mapHeight * 16 * constants::scale);

  player_character_ = new Character(4);

  input_command_ = new Command(player_character_);
  input_command_->InitializeProjectilePool(&collision_handler_);

  game_manager_.SetCollisionHandler(&collision_handler_);
  game_manager_.SetSceneCamera(&camera_);

  ObjectManager::GetInstance().SetGameManager(&game_manager_);
  EnemyManager::GetInstance().SetGameManager(&game_manager_);

  game_manager_.RegisterCharacterWithCollision(player_character_);
  ObjectManager::GetInstance().Reset(
    (int)constants::scale, &collision_handler_);

  game_manager_.SetCurrentLevel(current_level_);
  if (current_level_ == 1) {
    GameInfo::GetInstance().coin = 0;
  } else {
    if (GameInfo::GetInstance().HasInitialPointsForLevel(current_level_)) {
      GameInfo::GetInstance().coin
        = GameInfo::GetInstance().GetInitialPointsForLevel(current_level_);
    } else {
      GameInfo::GetInstance().SetInitialPointsForLevel(
        current_level_, GameInfo::GetInstance().coin);
    }
  }
  game_manager_.SetCharacter(player_character_);
  // if no custom level path is set, load default level based on current_level_
  if (!is_level_loaded_) {
    levelPath = "res/maps/map" + std::to_string(current_level_) + ".json";
  }
  game_manager_.LoadLevel(levelPath);
  player_character_->SetCharacter(character_type_, {10.0f, 500.0f});
}

void GameScene::Update() {
  if (IsKeyPressed(KEY_ESCAPE)) {
    App.AddScene(std::make_unique<PauseScene>(), false);
    return;
  }
  if (IsKeyPressed(KEY_P))
    game_manager_.TogglePause();

  if (game_manager_.IsPaused() || game_manager_.IsInTransition())
    return;
  if (player_character_) {
    // Check if character fell off the map
    if (
      player_character_->GetRectangle().y
      > constants::mapHeight * constants::blockSize) {
      // Only decrease life if the character wasn't already dead
      // This prevents double-counting deaths when killed by enemies
      game_manager_.OnPlayerDeath(player_character_, DeathCause::Fall);
      

      int currentLevel = game_manager_.GetCurrentLevel();
      // Check if we still have lives left
      if (is_level_loaded_) {
        GameInfo::GetInstance().coin = 0;
        const std::string reloadPath = levelPath; // copy before scene removal
        App.RemoveScene();
        App.AddScene(
          std::make_unique<GameScene>(
            CharacterSelectorScene::GetCharacterType(), reloadPath));
        return;
      } else if (GameInfo::GetInstance().life <= 0) {
        // Game over - reset game completely
        FileHandler::SaveGameInfo(GameInfo::GetInstance());
        GameInfo::GetInstance().Reset();
        // Load first level with scene replacement
        App.RemoveScene();
        App.AddScene(
          std::make_unique<GameOverScene>(
            GameOverScene()));
        return;
      } else {
        // Still have lives - restart current level with scene replacement
        // DON'T save current points! Instead, reset to initial level points
        if (currentLevel == 1) {
          // For level 1, always reset to 0
          GameInfo::GetInstance().coin = 0;
        } else {
          // For other levels, use initial level points (from when the player
          // first entered)
          if (GameInfo::GetInstance().HasInitialPointsForLevel(currentLevel)) {
            GameInfo::GetInstance().coin
              = GameInfo::GetInstance().GetInitialPointsForLevel(currentLevel);
          }
        }
        App.RemoveScene();
        App.AddScene(
          std::make_unique<GameScene>(
            CharacterSelectorScene::GetCharacterType(), currentLevel));

        return;
      }
    }
  }

  if (input_command_)
    input_command_->HandleInput();

  collision_handler_.CheckCollision();

  if (player_character_) {
    player_character_->SetFrameCount();
    player_character_->Update();
  }
  ObjectManager::GetInstance().SetFrameCount();
  ObjectManager::GetInstance().Update();
  input_command_->UpdateProjectiles();

  UpdateCamera(player_character_);

  float dt = GetFrameTime();
  game_manager_.Update(dt, player_character_);

  if (game_manager_.IsLevelComplete() && IsKeyPressed(KEY_SPACE)) {
    if (is_level_loaded_) {
      GameInfo::GetInstance().coin = 0;
      std::string reloadPath       = levelPath;
      App.RemoveScene();
      App.AddScene(
        std::make_unique<GameScene>(
          CharacterSelectorScene::GetCharacterType(), reloadPath));
    } else if (game_manager_.CanAdvanceLevel()) {
      GameInfo::GetInstance().coin = game_manager_.GetPoints();

      // Award for level completion
      GameInfo::GetInstance().life++;

      int nextLevel = game_manager_.GetCurrentLevel() + 1;

      App.RemoveScene();
      App.AddScene(
        std::make_unique<GameScene>(
          CharacterSelectorScene::GetCharacterType(), nextLevel));
    } else {
      // All levels completed, restart game
      GameInfo::GetInstance().Reset(); // Reset game info
      App.RemoveScene();
      App.AddScene(
        std::make_unique<GameScene>(
          CharacterSelectorScene::GetCharacterType(), 1)); // Back to level 1
    }
    return;
  }
}

void GameScene::Draw() {
  // Let game manager handle the background drawing

  BeginMode2D(camera_);

  // Let game manager handle drawing objects and enemies with proper background
  game_manager_.DrawLevel();

  // Draw the player character (Mario or Luigi based on selection)
  if (player_character_)
    player_character_->Draw();

  // Draw projectiles from Command
  if (input_command_)
    input_command_->DrawProjectiles();

  EndMode2D();

  // UI is drawn by game manager in DrawLevel()
  // But we also draw instructions system from Command
  if (input_command_) {
    input_command_->DrawInstructionsButton();
    input_command_->DrawInstructionsPanel();
  }
}

void GameScene::Resume() {
  game_manager_.ResumeEnemies();
}

// void GameScene::UpdateCamera(Character *character) {
//   if (!character)
//     return;
//
//   Rectangle player_rect = character->GetRectangle();
//   Vector2 player_pos    = {
//     player_rect.x + player_rect.width / 2,
//     player_rect.y + player_rect.height / 2};
//
//   float visible_width  = constants::screenWidth;
//   float visible_height = constants::screenHeight;
//   float map_width      = constants::mapWidth * constants::blockSize;
//   float map_height     = constants::mapHeight * constants::blockSize;
//
//   float smooth = 5 * GetFrameTime();
//
//   // Dead zone
//   float left_boundary     = 50;
//   float right_boundary    = constants::screenWidth / 2;
//   float desired_x         = player_pos.x;
//   float player_pos_screen = player_pos.x - camera_.target.x +
//   camera_.offset.x;
//
//   if (player_pos_screen < left_boundary)
//     desired_x = player_pos.x - (left_boundary - camera_.offset.x);
//   else if (player_pos_screen + player_rect.width > right_boundary)
//     desired_x
//       = player_pos.x + player_rect.width - (right_boundary -
//       camera_.offset.x);
//
//   camera_.target.x = Lerp(camera_.target.x, desired_x, smooth);
//   camera_.target.y = Lerp(camera_.target.y, player_pos.y, smooth);
//
//   camera_.target.x
//     = Clamp(camera_.target.x, visible_width / 2, map_width - visible_width /
//     2);
//   camera_.target.y = Clamp(
//     camera_.target.y, visible_height / 2, map_height - visible_height / 2);
// }

void GameScene::UpdateCamera(Character *character) {
  if (!character)
    return;

  Vector2 desired_target = camera_.target;

  Rectangle player_rect = character->GetRectangle();
  Vector2 player_center = {
    player_rect.x + player_rect.width / 2,
    player_rect.y + player_rect.height / 2};

  float visible_width  = constants::screenWidth;
  float visible_height = constants::screenHeight;
  float map_width      = constants::mapWidth * constants::blockSize;
  float map_height     = constants::mapHeight * constants::blockSize;

  float smooth = 10 * GetFrameTime();

  // Dead zone
  Vector2 player_screen = GetWorldToScreen2D(player_center, camera_);
  float left_boundary   = visible_width / 4;
  float right_boundary  = visible_width / 3;
  float top_boundary    = visible_height / 5;
  float bottom_boundary = visible_height / 3;

  if (player_screen.x < left_boundary)
    desired_target.x -= (left_boundary - player_screen.x);
  else if (player_screen.x > right_boundary)
    desired_target.x += (player_screen.x - right_boundary);

  if (player_screen.y < top_boundary)
    desired_target.y -= (top_boundary - player_screen.y);
  else if (player_screen.y > bottom_boundary)
    desired_target.y += (player_screen.y - bottom_boundary);

  camera_.target.x = Lerp(camera_.target.x, desired_target.x, smooth);
  camera_.target.y = Lerp(camera_.target.y, desired_target.y, smooth);

  camera_.target.x
    = Clamp(camera_.target.x, visible_width / 2, map_width - visible_width / 2);
  camera_.target.y = Clamp(
    camera_.target.y, visible_height / 2, map_height - visible_height / 2);
}

SceneType GameScene::Type() {
  return type_;
}

void GameScene::SetLevelPath(const std::string &path) {
  levelPath        = path;
  is_level_loaded_ = true;
}
