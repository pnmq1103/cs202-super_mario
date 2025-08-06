#include <raylib.h>
#include <stdexcept>

#include "include/characters/character.hpp"
#include "include/core/application.hpp"
#include "include/core/character_selector.hpp"
#include "include/core/command.hpp"
#include "include/core/game.hpp"
#include "include/core/game_managing.hpp"
#include "include/core/pause.hpp"
#include "include/managers/enemy_manager.hpp"
#include "include/objects/object_manager.hpp"

CollisionHandler *GameScene::collision_handler_ = nullptr;

GameScene::GameScene(CharacterType type)
    : game_manager_(), character_type_(type) {}

GameScene::~GameScene() {
  EnemyManager::GetInstance().ClearAllEnemies();
  if (collision_handler_) {
    collision_handler_->Reset(256 * 4, 240 * 4);
    ObjectManager::GetInstance().Reset(4, collision_handler_);
  }

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

  camera_.offset   = {GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};
  camera_.rotation = 0.0f;
  camera_.zoom     = 1.0f;
  // Position camera at a better starting position
  camera_.target = {100.0f, 500.0f};
  

  if (!collision_handler_) {
    collision_handler_ = new CollisionHandler(256 * 4, 240 * 4);
  } else {
    collision_handler_->Reset(256 * 4, 240 * 4);
  }

  player_character_ = new Character(4);

  input_command_ = new Command(player_character_);
  input_command_->InitializeProjectilePool(collision_handler_);

  game_manager_.InitializeCollisionSystem(256 * 4, 240 * 4);
  game_manager_.SetCollisionHandler(collision_handler_);
  game_manager_.SetSceneCamera(&camera_);

  game_manager_.RegisterCharacterWithCollision(player_character_);
  game_manager_.LoadLevel("res/maps/map3.json");
  //CreateSimpleTestLevel();
  player_character_->SetCharacter(character_type_, {10.0f, 500.0f});
}

void GameScene::Update() {
  if (IsKeyPressed(KEY_ESCAPE)) {
    App.AddScene(std::make_unique<PauseScene>(), false);
    return;
  }
  if (IsKeyPressed(KEY_P)) {
    game_manager_.TogglePause();
  }

  if (game_manager_.IsPaused() || game_manager_.IsInTransition()) {
    return;
  }

  if (input_command_) {
    input_command_->HandleInput();
  }

  collision_handler_->CheckCollision();
  player_character_;

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

  // Handle level progression
  game_manager_.HandleLevelProgression();

  // Handle game over/restart (improved)
  if (game_manager_.IsGameOver() && IsKeyPressed(KEY_R)) {
    game_manager_.ResetGame();
    // Reload first level
    game_manager_.LoadLevel("res/maps/map1.json");
  }

  // Handle level completion (improved)
  if (game_manager_.IsLevelComplete() && IsKeyPressed(KEY_SPACE)) {
    if (game_manager_.CanAdvanceLevel()) {
      game_manager_.LoadNextLevel();
    } else {
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
  if (player_character_) {
    player_character_->Draw();
  }

  // Draw projectiles from Command
  if (input_command_) {
    input_command_->DrawProjectiles();
  }

  EndMode2D();

  // UI is drawn by game manager in DrawLevel()
  // But we also draw instructions system from Command
  if (input_command_) {
    input_command_->DrawInstructionsButton();
    input_command_->DrawInstructionsPanel();
  }
}

void GameScene::Resume() {
  // Resume any paused systems
  game_manager_.ResumeEnemies();

  // Resume background music if it was paused
  App.Media().PlayMusic("ground_theme");
}

void GameScene::UpdateCamera(Character *character) {
    // Quan take care
}
SceneType GameScene::Type() {
  return type_;
}
void GameScene::CreateSimpleTestLevel() {
  // Initialize managers first
  ObjectManager &objectManager = ObjectManager::GetInstance();
  objectManager.Reset(4, collision_handler_);

  EnemyManager &enemyManager = EnemyManager::GetInstance();
  enemyManager.SetCollisionHandler(collision_handler_);

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