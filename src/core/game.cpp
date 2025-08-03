#include <raylib.h>
#include <stdexcept>

#include "include/characters/character.hpp"
#include "include/core/application.hpp"
#include "include/core/command.hpp"
#include "include/core/game.hpp"
#include "include/core/game_managing.hpp"
#include "include/managers/enemy_manager.hpp"
#include "include/objects/object_manager.hpp"

GameScene::GameScene() : game_manager_() {}

GameScene::~GameScene() {
  // Clear managers first to prevent accessing destroyed objects
  EnemyManager::GetInstance().ClearAllEnemies();
  
  // Reset ObjectManager to clear its objects
  if (collision_handler_) {
    ObjectManager::GetInstance().Reset(4, collision_handler_);
  }
  
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

  if (collision_handler_) {
    delete collision_handler_;
    collision_handler_ = nullptr;
  }
}

void GameScene::Init() {
  App.Media().PlayMusic("ground_theme");

  camera_.target = {100.0f, 300.0f};
  camera_.offset = {GetScreenWidth()/2.0f, GetScreenHeight()/2.0f};
  camera_.rotation = 0.0f;
  camera_.zoom = 1.0f;

  collision_handler_ = new CollisionHandler(3200.0f, 1000.0f);

  // Create Mario character
  mario_character_ = new Character(4.0f);
  mario_character_->SetCharacter(CharacterType::MARIO, {100.0f, 570.0f}); // Match spawn point

  // Create Luigi character  
  luigi_character_ = new Character(4.0f);
  luigi_character_->SetCharacter(CharacterType::LUIGI, {150.0f, 570.0f}); // Slightly offset from Mario

  // Create command handler with both characters (Mario starts as active)
  input_command_ = new Command(mario_character_, luigi_character_);
  input_command_->InitializeProjectilePool(collision_handler_); // Initialize projectile pool in Command

  // Initialize collision system in game manager with proper world size
  game_manager_.InitializeCollisionSystem(3200.0f, 1000.0f);
  
  // Set collision handler and camera reference for game manager
  game_manager_.SetCollisionHandler(collision_handler_);
  game_manager_.SetSceneCamera(&camera_); // Use inherited camera

  // Register characters with collision system
  game_manager_.RegisterCharacterWithCollision(mario_character_);
  game_manager_.RegisterCharacterWithCollision(luigi_character_);

  // Create a simple test level instead of trying to load JSON
  CreateSimpleTestLevel();
}

void GameScene::CreateSimpleTestLevel() {
  // Initialize managers first
  ObjectManager& objectManager = ObjectManager::GetInstance();
  objectManager.Reset(4, collision_handler_);
  
  EnemyManager& enemyManager = EnemyManager::GetInstance();
  enemyManager.SetCollisionHandler(collision_handler_);

  // Create ground - extended for a proper level
  for (int x = 0; x < 200; x += 64) {
    objectManager.AddStaticBlock({(float)x, 600.0f}, 'g'); // Ground blocks
  }
  
  // Add some platforms
  for (int x = 300; x < 500; x += 64) {
    objectManager.AddStaticBlock({(float)x, 400.0f}, 'g'); // Platform
  }
  
  // Add some blocks and power-ups
  objectManager.AddBrickBlock({400.0f, 300.0f});
  objectManager.AddQuestionBlock({500.0f, 300.0f}, QuestionBlockItem::coin);
  objectManager.AddQuestionBlock({600.0f, 300.0f}, QuestionBlockItem::super_mushroom);
  
  // Add some walls for collision testing
  for (int i = 0; i < 10; i++) {
    objectManager.AddStaticBlock({0.0f, 600.0f - 64.0f * (float)i}, 'g'); // Left wall
    objectManager.AddStaticBlock({3136.0f, 600.0f - 64.0f * (float)i}, 'g'); // Right wall at map edge
  }

  // Spawn some enemies for testing
  enemyManager.SpawnEnemy(EnemyType::Goomba, {200.0f, 550.0f});
  enemyManager.SpawnEnemy(EnemyType::Koopa, {400.0f, 550.0f});
  enemyManager.SpawnEnemy(EnemyType::Piranha, {600.0f, 570.0f});
  enemyManager.SpawnEnemy(EnemyType::Goomba, {800.0f, 550.0f});

  // Set up initial movement for enemies
  const std::vector<Enemy*>& enemies = enemyManager.GetEnemies();
  for (Enemy* enemy : enemies) {
    if (enemy) {
      switch (enemy->GetType()) {
        case EnemyType::Goomba:
          enemy->SetVelocity({30.0f, 0.0f}); // Moving right
          enemy->SetFacing(false);
          break;
        case EnemyType::Koopa:
          enemy->SetVelocity({25.0f, 0.0f}); // Moving right
          enemy->SetFacing(false);
          break;
        case EnemyType::Piranha:
          enemy->SetVelocity({0.0f, 0.0f}); // Stationary
          break;
        default:
          break;
      }
    }
  }

  std::cout << "Simple test level created with " << enemies.size() << " enemies" << std::endl;
}

void GameScene::Update() {
  if (IsKeyPressed(KEY_ESCAPE)) {
    App.RemoveScene();
    return;
  }

  // Handle character input through command system (includes instructions handling)
  if (input_command_) {
    input_command_->HandleInput();
    // Command now handles projectile updates internally
    input_command_->UpdateProjectiles();
  }

  // Get active character
  Character *active_character = input_command_->GetActiveCharacter();

  // Update character frame count and physics
  if (active_character) {
    active_character->SetFrameCount();
    active_character->Update();
    
    // Add ground collision for character
    Rectangle charRect = active_character->GetRectangle();
    if (charRect.y + charRect.height >= 600.0f) { // Ground level
      active_character->StopY(600.0f);
    }
  }

  // Update camera to follow active character
  UpdateCamera(active_character);

  // Update game manager with enemies and collision system
  float dt = GetFrameTime();
  game_manager_.Update(dt, active_character);

  // Handle level progression (if GameManaging has these methods)
  game_manager_.HandleLevelProgression();

  // Simple game over/restart handling
  if (game_manager_.IsGameOver() && IsKeyPressed(KEY_R)) {
    game_manager_.ResetGame();
    CreateSimpleTestLevel(); // Recreate the test level
  }

  if (game_manager_.IsLevelComplete() && IsKeyPressed(KEY_SPACE)) {
    // For now, just recreate the same level
    CreateSimpleTestLevel();
  }
}

void GameScene::Draw() {
  // Let game manager handle the background drawing
  
  BeginMode2D(camera_);
  
  // Let game manager handle drawing objects and enemies with proper background
  game_manager_.DrawLevel();
  
  // Draw characters (Mario/Luigi) - IMPORTANT: Must draw characters here
  if (mario_character_) {
    mario_character_->Draw();
  }
  if (luigi_character_ && luigi_character_ != mario_character_) {
    luigi_character_->Draw(); 
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

void GameScene::UpdateCamera(Character* character) {
  if (!character) return;

  Rectangle charRect = character->GetRectangle();
  Vector2 charPos = {charRect.x + charRect.width / 2, charRect.y + charRect.height / 2};

  // Smooth camera following
  float smoothing = 0.1f;
  Vector2 targetPos = {charPos.x, charPos.y - 100.0f};

  camera_.target.x += (targetPos.x - camera_.target.x) * smoothing;
  camera_.target.y += (targetPos.y - camera_.target.y) * smoothing;

  // Keep camera within level bounds
  float levelWidth = 3200.0f;
  float levelHeight = 1000.0f;
  float halfScreenWidth = GetScreenWidth() / 2.0f / camera_.zoom;
  float halfScreenHeight = GetScreenHeight() / 2.0f / camera_.zoom;

  camera_.target.x = fmaxf(halfScreenWidth, 
    fminf(camera_.target.x, levelWidth - halfScreenWidth));
  camera_.target.y = fmaxf(halfScreenHeight, 
    fminf(camera_.target.y, levelHeight - halfScreenHeight));
}

SceneType GameScene::Type() {
  return type_;
}
