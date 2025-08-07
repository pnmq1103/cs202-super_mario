#include <raylib.h>
#include <raymath.h>
#include <stdexcept>

#include "include/characters/character.hpp"
#include "include/core/application.hpp"
#include "include/core/character_selector.hpp"
#include "include/core/command.hpp"
#include "include/core/constants.hpp"
#include "include/core/game.hpp"
#include "include/core/game_managing.hpp"
#include "include/core/pause.hpp"
#include "include/managers/enemy_manager.hpp"
#include "include/objects/object_manager.hpp"

CollisionHandler GameScene::collision_handler_(
  constants::mapWidth * 16 * constants::scale,
  constants::mapHeight * 16 * constants::scale);

GameScene::GameScene(CharacterType type)
    : game_manager_(), character_type_(type) {}

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
}

void GameScene::Init() {
  App.Media().PlayMusic("ground_theme");

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
  game_manager_.LoadLevel("res/maps/map2.json");
  // CreateSimpleTestLevel();
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
 
  //if (player_character_) {
  //  static bool wasAlive = true;

  //  // Use position and velocity instead of direct state access
  //  bool isCurrentlyAlive = true;

  //  // Character is dead if:
  //  // 1. It's not moving when it should be (check velocity)
  //  Vector2 velocity = player_character_->GetSpeed();
  //  // 2. Check if y position is too low (fell off map)
  //  Rectangle rect = player_character_->GetRectangle();
  //  float deathY   = 1500.0f; // Adjust this value based on your map

  //  // Consider character dead if it fell below threshold
  //  if (rect.y > deathY) {
  //    isCurrentlyAlive = false;
  //  }

  //  // If character was alive but now is dead, handle death
  //  if (wasAlive && !isCurrentlyAlive) {
  //    game_manager_.OnPlayerDeath(player_character_);
  //    wasAlive = false;
  //    return;
  //  }

  //  if (isCurrentlyAlive) {
  //    wasAlive = true;
  //  }
  //}

  if (input_command_)
    input_command_->HandleInput();

  collision_handler_.CheckCollision();

  if (player_character_) {
    player_character_->SetFrameCount();
    player_character_->Update();
  }
  ObjectManager::GetInstance().SetFrameCount();
  ObjectManager::GetInstance().Update();
  Enemy::SetFrameCount();
  input_command_->UpdateProjectiles();

  UpdateCamera(player_character_);

  // Let game manager handle all game logic
  float dt = GetFrameTime();
  game_manager_.Update(dt, player_character_);

  game_manager_.HandleLevelProgression();

  // Game over/restart
  if (game_manager_.IsGameOver() && IsKeyPressed(KEY_R)) {
    game_manager_.ResetGame();
    // Reload first level
    game_manager_.LoadLevel("res/maps/map1.json");
  }

  // Level completion
  if (game_manager_.IsLevelComplete() && IsKeyPressed(KEY_SPACE)) {
    if (game_manager_.CanAdvanceLevel())
      game_manager_.LoadNextLevel();
    else {
      // All levels completed, restart game
      game_manager_.ResetGame();
      game_manager_.LoadLevel("res/maps/map1.json");
    }
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

void GameScene::UpdateCamera(Character *character) {
  if (!character)
    return;

  Rectangle player_rect = character->GetRectangle();
  Vector2 player_pos    = {
    player_rect.x + player_rect.width / 2,
    player_rect.y + player_rect.height / 2};

  float visible_width  = constants::screenWidth;
  float visible_height = constants::screenHeight;
  float map_width      = constants::mapWidth * constants::blockSize;
  float map_height     = constants::mapHeight * constants::blockSize;

  float smooth = 5 * GetFrameTime();

  // Dead zone
  float left_boundary     = 50;
  float right_boundary    = constants::screenWidth / 2;
  float desired_x         = player_pos.x;
  float player_pos_screen = player_pos.x - camera_.target.x + camera_.offset.x;

  if (player_pos_screen < left_boundary)
    desired_x = player_pos.x - (left_boundary - camera_.offset.x);
  else if (player_pos_screen + player_rect.width > right_boundary)
    desired_x
      = player_pos.x + player_rect.width - (right_boundary - camera_.offset.x);

  camera_.target.x = Lerp(camera_.target.x, desired_x, smooth);
  camera_.target.y = Lerp(camera_.target.y, player_pos.y, smooth);

  camera_.target.x
    = Clamp(camera_.target.x, visible_width / 2, map_width - visible_width / 2);
  camera_.target.y = Clamp(
    camera_.target.y, visible_height / 2, map_height - visible_height / 2);
}

SceneType GameScene::Type() {
  return type_;
}

void GameScene::CreateSimpleTestLevel() {
  // Initialize managers first
  ObjectManager &objectManager = ObjectManager::GetInstance();
  objectManager.Reset(4, &collision_handler_);

  EnemyManager &enemyManager = EnemyManager::GetInstance();
  enemyManager.SetCollisionHandler(&collision_handler_);

  // Create ground - extended for a proper level
  for (int x = 0; x < 200; x += 64) {
    objectManager.AddStaticBlockByTheme(
      {(float)x, 600.0f}, 'g'); // Ground blocks
  }

  // Add some platforms
  for (int x = 300; x < 500; x += 64) {
    objectManager.AddStaticBlockByTheme({(float)x, 400.0f}, 'g'); // Platform
  }

  // Add some blocks and power-ups
  objectManager.AddBrickBlock({400.0f, 300.0f});
  objectManager.AddQuestionBlock({500.0f, 300.0f}, QuestionBlockItem::coin);
  objectManager.AddQuestionBlock(
    {600.0f, 300.0f}, QuestionBlockItem::super_mushroom);

  // Add some walls for collision testing
  for (int i = 0; i < 10; i++) {
    objectManager.AddStaticBlockByTheme(
      {0.0f, 600.0f - 64.0f * (float)i}, 'g'); // Left wall
    objectManager.AddStaticBlockByTheme(
      {3136.0f, 600.0f - 64.0f * (float)i}, 'g'); // Right wall at map edge
  }

  // Spawn some enemies for testing
  enemyManager.SpawnEnemy(EnemyType::Goomba, {200.0f, 550.0f});
  // enemyManager.SpawnEnemy(EnemyType::Koopa, {400.0f, 550.0f});
  // enemyManager.SpawnEnemy(EnemyType::Piranha, {600.0f, 570.0f});
  // enemyManager.SpawnEnemy(EnemyType::Goomba, {800.0f, 550.0f});

  //// Set up initial movement for enemies
  // const std::vector<Enemy *> &enemies = enemyManager.GetEnemies();
  // for (Enemy *enemy : enemies) {
  //   if (enemy) {
  //     switch (enemy->GetType()) {
  //       case EnemyType::Goomba:
  //         enemy->SetVelocity({30.0f, 0.0f}); // Moving right
  //         enemy->SetFacing(false);
  //         break;
  //       case EnemyType::Koopa:
  //         enemy->SetVelocity({25.0f, 0.0f}); // Moving right
  //         enemy->SetFacing(false);
  //         break;
  //       case EnemyType::Piranha:
  //         enemy->SetVelocity({0.0f, 0.0f}); // Stationary
  //         break;
  //       default:
  //         break;
  //     }
  //   }
  // }
}