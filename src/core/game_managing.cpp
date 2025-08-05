#include "include/core/game_managing.hpp"
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <raylib.h>
#include <stdexcept>

#include "include/characters/character.hpp"
#include "include/core/application.hpp"
#include "include/core/constants.hpp"
#include "include/core/file_handler.hpp"
#include "include/core/map.hpp"
#include "include/external/json.hpp"
#include "include/managers/enemy_manager.hpp"
#include "include/objects/object_manager.hpp"

using json = nlohmann::json;

GameManaging::GameManaging() {
  // Basic game state initialization
  lives_            = 3;
  gameTime_         = 0.0f;
  points_           = 0;
  countdownSeconds_ = 300;
  gameOver_         = false;
  levelComplete_    = false;
  spawnPoint_       = {100.0f, 500.0f};

  // Level progression
  currentLevel_ = 1;
  maxLevels_    = 3;

  // Camera setup
  sceneCamera_ = nullptr;

  // Level boundaries
  levelWidth_  = 3200.0f;
  levelHeight_ = 800.0f;
  groundLevel_ = 600.0f;

  // Collision system
  collisionHandler_ = nullptr;

  // Level completion criteria
  levelEndX_
    = 3000.0f; // Character must reach this X position to complete level
  totalEnemies_  = 0;
  enemiesKilled_ = 0;
}

GameManaging::~GameManaging() {
  UnloadLevel();
}

void GameManaging::SetSceneCamera(Camera2D *camera) {
  sceneCamera_ = camera;
}

void GameManaging::SetCollisionHandler(CollisionHandler *handler) {
  collisionHandler_ = handler;
  if (collisionHandler_) {
    collisionHandler_->Reset(levelWidth_, levelHeight_);
    EnemyManager::GetInstance().SetCollisionHandler(collisionHandler_);
  }
}

void
GameManaging::InitializeCollisionSystem(float worldWidth, float worldHeight) {
  levelWidth_  = worldWidth;
  levelHeight_ = worldHeight;

  if (collisionHandler_) {
    collisionHandler_->Reset(levelWidth_, levelHeight_);
  }
}

void GameManaging::RegisterCharacterWithCollision(Character *character) {
  if (collisionHandler_ && character) {
    collisionHandler_->AddCharacter(character);
  }
}

// Lac lam
void GameManaging::LoadLevel(const std::string &filename) {
  std::cout << "Loading level: " << filename << std::endl;

  UnloadLevel();

  // Set frame counters like in main.cpp
  ObjectManager::GetInstance().SetFrameCount();
  Enemy::SetFrameCount();

  // IMPORTANT: Make sure collision handler is properly initialized BEFORE Reset
  if (!collisionHandler_) {
    // Initialize collision system if not already done
    InitializeCollisionSystem(levelWidth_, levelHeight_);
  }

  // Reset managers
  if (collisionHandler_) {
    collisionHandler_->Reset(levelWidth_, levelHeight_);
    // Set collision handler for EnemyManager
    EnemyManager::GetInstance().SetCollisionHandler(collisionHandler_);
  }

  // Reset ObjectManager with the VALID collision handler
  ObjectManager::GetInstance().Reset((int)constants::scale, collisionHandler_);

  // Reset level completion tracking
  levelComplete_ = false;
  totalEnemies_  = 0;
  enemiesKilled_ = 0;

  // Try to load from JSON file
  try {
    Map map;
    FileHandler::LoadMapFromFile(map, filename);

    // Process map data to create game objects
    int mapSize = constants::mapWidth * constants::mapHeight;
    for (int i = 0; i < mapSize; i++) {
      // Create blocks from Tile1 layer
      int tileGid        = map.GetCell(MapLayer::Tile1, i);
      int pipeGid        = map.GetCell(MapLayer::Tile2, i);
      Rectangle tileRect = {0, 0, 0, 0}; // Initialize tile rectangle
      if (tileGid != 0) {
        tileRect             = map.GetInfo(tileGid);
        Vector2 tilePosition = {
          (float)(i % constants::mapWidth) * constants::blockSize,
          (float)(i / constants::mapWidth) * constants::blockSize};
        if (tileGid == 1) {
          ObjectManager::GetInstance().AddBrickBlock(tilePosition);
        } else if (tileGid == 2) {
          ObjectManager::GetInstance().AddQuestionBlock(
            tilePosition, QuestionBlockItem::coin);
        } else if (tileGid >= 17 && tileGid <= 36) { // using the minimal
                                                     // tileset
          ObjectManager::GetInstance().AddStaticBlockByGid(
            tilePosition,
            tileRect); // add by gid instead of theme's default block
        } else {
          // fallback for debugging
          std::cout << "Unknown tile GID: " << tileGid << " at index " << i
                    << std::endl;
          ObjectManager::GetInstance().AddStaticBlockByTheme(
            tilePosition, 'g'); // default to ground block
        }
      }
      // Create enemies from Objects layer
      int enemyGid = map.GetCell(MapLayer::Objects, i);
      if (enemyGid != 0) {
        Vector2 enemyPosition = {
          (float)(i % constants::mapWidth) * constants::blockSize,
          (float)(i / constants::mapWidth) * constants::blockSize};
        CreateEnemyFromType(enemyGid, enemyPosition);
        totalEnemies_++; // Count enemies for completion tracking
      }

      // draw the pipes last so they are on top of the piranha plants
      if (pipeGid != 0) {
        Vector2 pipePosition = {
          (float)(i % constants::mapWidth) * constants::blockSize,
          (float)(i / constants::mapWidth) * constants::blockSize};
        // hard-code pipe blocks; all pipes in map will be the same size
        ObjectManager::GetInstance().AddPipeBlock(
          pipePosition, constants::blockSize * 3, true, true, false);
      }
      // Handle special objects from Tile2 layer (spawn points, goal flags,
      // etc.)
      /*int specialGid = map.GetCell(MapLayer::Tile2, i);
      if (specialGid != 0) {
        Vector2 specialPosition = {
          (float)(i % constants::mapWidth) * constants::blockSize,
          (float)(i / constants::mapWidth) * constants::blockSize
        };
        CreateSpecialObjectFromType(specialGid, specialPosition);
      }*/
    }

    std::cout << "Level loaded successfully. Total enemies: " << totalEnemies_
              << std::endl;

  } catch (const std::exception &e) {
    std::cerr << "Failed to load level " << filename << ": " << e.what()
              << std::endl;
    // Create a fallback level
    CreateFallbackLevel();
  }

  // Set camera target to spawn point if available
  if (sceneCamera_) {
    sceneCamera_->target = spawnPoint_;
  }

  // Apply level difficulty modifiers
  ApplyLevelDifficulty();
}

void GameManaging::CreateFallbackLevel() {
  std::cout << "Creating fallback level..." << std::endl;

  ObjectManager &objectManager = ObjectManager::GetInstance();
  EnemyManager &enemyManager   = EnemyManager::GetInstance();

  // Create a simple platform level
  for (int x = 0; x < 200; x += 32) {
    objectManager.AddStaticBlockByTheme(
      {(float)x, 600.0f}, 'g'); // Ground blocks
  }

  // Add some question blocks
  objectManager.AddQuestionBlock({300.0f, 400.0f}, QuestionBlockItem::coin);
  objectManager.AddQuestionBlock({500.0f, 400.0f}, QuestionBlockItem::coin);
  objectManager.AddQuestionBlock({700.0f, 400.0f}, QuestionBlockItem::coin);

  // Add some brick blocks
  objectManager.AddBrickBlock({400.0f, 400.0f});
  objectManager.AddBrickBlock({600.0f, 400.0f});

  // Add enemies based on current level
  int baseEnemyCount = 3 + (currentLevel_ - 1) * 2; // More enemies per level
  for (int i = 0; i < baseEnemyCount; i++) {
    Vector2 enemyPos = {200.0f + i * 150.0f, 550.0f};

    if (currentLevel_ == 1) {
      enemyManager.SpawnEnemy(EnemyType::Goomba, enemyPos);
    } else if (currentLevel_ == 2) {
      if (i % 2 == 0) {
        enemyManager.SpawnEnemy(EnemyType::Goomba, enemyPos);
      } else {
        enemyManager.SpawnEnemy(EnemyType::Koopa, enemyPos);
      }
    } else { // Level 3
      if (i % 3 == 0) {
        enemyManager.SpawnEnemy(EnemyType::Piranha, enemyPos);
      } else if (i % 3 == 1) {
        enemyManager.SpawnEnemy(EnemyType::Koopa, enemyPos);
      } else {
        enemyManager.SpawnEnemy(EnemyType::Goomba, enemyPos);
      }
    }
    totalEnemies_++;

    // Set up proper initial movement for newly spawned enemies
    const std::vector<Enemy *> &enemies = enemyManager.GetEnemies();
    if (!enemies.empty()) {
      Enemy *lastEnemy = enemies.back();
      if (lastEnemy) {
        // Set initial movement direction based on enemy type
        switch (lastEnemy->GetType()) {
          case EnemyType::Goomba:
            lastEnemy->SetVelocity({30.0f, 0.0f}); // Start moving right
            lastEnemy->SetFacing(false);           // Face right
            break;
          case EnemyType::Koopa:
            lastEnemy->SetVelocity({25.0f, 0.0f}); // Start moving right
            lastEnemy->SetFacing(false);           // Face right
            break;
          case EnemyType::Bowser:
            lastEnemy->SetVelocity({15.0f, 0.0f}); // Start moving right slower
            lastEnemy->SetFacing(false);           // Face right
            break;
          case EnemyType::Piranha:
            // Piranha doesn't move horizontally
            lastEnemy->SetVelocity({0.0f, 0.0f});
            lastEnemy->SetFacing(false);
            break;
        }
        std::cout << "Set up enemy " << static_cast<int>(lastEnemy->GetType())
                  << " with velocity: " << lastEnemy->GetVelocity().x
                  << std::endl;
      }
    }
  }

  // Set level end position
  levelEndX_
    = 1800.0f
      + (currentLevel_ - 1) * 400.0f; // Longer levels for higher difficulties
}

void GameManaging::ApplyLevelDifficulty() {
  // Adjust game parameters based on level difficulty
  switch (currentLevel_) {
    case 1:
      countdownSeconds_ = 400; // More time for level 1
      break;
    case 2:
      countdownSeconds_ = 350; // Less time for level 2
      break;
    case 3:
      countdownSeconds_ = 300; // Even less time for level 3
      break;
    default:
      countdownSeconds_ = 300;
      break;
  }

  // Reset game time for new level
  gameTime_ = 0.0f;

  std::cout << "Level " << currentLevel_
            << " difficulty applied. Time limit: " << countdownSeconds_ << "s"
            << std::endl;
}

void GameManaging::UnloadLevel() {
  particles_.clear();

  // Clear managers
  EnemyManager::GetInstance().ClearAllEnemies();
  // ObjectManager doesn't have Clear method, so we reset it instead
  if (collisionHandler_) {
    ObjectManager::GetInstance().Reset(
      (int)constants::scale, collisionHandler_);
  }

  if (collisionHandler_) {
    collisionHandler_->Reset(levelWidth_, levelHeight_);
  }
}

void GameManaging::ResetGame() {
  lives_         = 3;
  points_        = 0;
  gameTime_      = 0.0f;
  gameOver_      = false;
  levelComplete_ = false;
  currentLevel_  = 1;
  spawnPoint_    = {100.0f, 500.0f};
  totalEnemies_  = 0;
  enemiesKilled_ = 0;
  UnloadLevel();
}

void GameManaging::Update(float deltaTime, Character *activeCharacter) {
  if (gameOver_ || levelComplete_)
    return;

  // Update time
  UpdateTime();

  // ObjectManager::GetInstance().SetFrameCount();
  // ObjectManager::GetInstance().Update();

  Enemy::SetFrameCount();
  EnemyManager &enemyManager = EnemyManager::GetInstance();
  enemyManager.UpdateAll(deltaTime);

  // Update enemy kill count for level completion
  int currentAliveEnemies = (int)enemyManager.GetEnemies().size();
  enemiesKilled_          = totalEnemies_ - currentAliveEnemies;

  // Clear dead enemies (killed by boundaries or other means)
  enemyManager.ClearDeadEnemies();

  // Update collision system - this will handle wall/boundary collisions for
  // enemies
  // UpdateCollisionSystem(); ********************* wrong order **************

  // Check level completion conditions
  CheckLevelCompletion(activeCharacter);

  // Update particles
  UpdateParticles(deltaTime);
  CleanupDeadParticles();

  // Update background music
  UpdateBackgroundMusic();
}

void GameManaging::CheckLevelCompletion(Character *activeCharacter) {
  if (!activeCharacter || levelComplete_)
    return;

  Rectangle charRect = activeCharacter->GetRectangle();

  // Check if character reached the end of the level
  bool reachedEnd = charRect.x >= levelEndX_;

  // Check if all enemies are defeated (optional completion method)
  bool allEnemiesDefeated
    = (totalEnemies_ > 0) && (enemiesKilled_ >= totalEnemies_);

  if (reachedEnd || allEnemiesDefeated) {
    levelComplete_ = true;
    AddPoints(1000 * currentLevel_); // Bonus points for completing level

    if (reachedEnd) {
      std::cout << "Level " << currentLevel_
                << " completed by reaching the end!" << std::endl;
    } else {
      std::cout << "Level " << currentLevel_
                << " completed by defeating all enemies!" << std::endl;
    }
  }
}

void GameManaging::UpdateCollisionSystem() {
  if (collisionHandler_) {
    collisionHandler_->CheckCollision();
  }
}

void GameManaging::DrawLevel() {
  if (!sceneCamera_)
    return;

  // BeginMode2D(*sceneCamera_);

  DrawBackground();

  // Draw blocks using ObjectManager
  ObjectManager::GetInstance().SetFrameCount();
  ObjectManager::GetInstance().Update();
  ObjectManager::GetInstance().Draw();

  // Draw enemies using EnemyManager
  EnemyManager &enemyManager = EnemyManager::GetInstance();
  for (Enemy *enemy : enemyManager.GetEnemies()) {
    if (enemy && enemy->IsAlive()) {
      enemy->Draw();
    }
  }

  // Draw particles
  for (const auto &particle : particles_) {
    DrawPixelV(particle.position, particle.color);
  }

  // Draw level end indicator
  DrawRectangle((int)levelEndX_, 0, 10, (int)levelHeight_, GREEN);

  // EndMode2D();

  // Draw UI
  DrawStats();
}

void GameManaging::DrawStats() const {
  DrawText(TextFormat("Lives: %d", lives_), 10, 10, 24, WHITE);
  DrawText(TextFormat("Points: %d", points_), 10, 40, 24, YELLOW);
  DrawText(
    TextFormat("Time: %d", countdownSeconds_ - (int)gameTime_), 10, 70, 24,
    GREEN);
  DrawText(
    TextFormat("Level: %d/%d", currentLevel_, maxLevels_), 10, 100, 24, BLUE);
  DrawText(
    TextFormat("Enemies: %d/%d", enemiesKilled_, totalEnemies_), 10, 130, 20,
    ORANGE);

  // Debug info
  DrawText(
    TextFormat("Spawn: %.0f, %.0f", spawnPoint_.x, spawnPoint_.y), 10, 160, 18,
    LIME);
  DrawText(TextFormat("Level End: %.0f", levelEndX_), 10, 190, 18, LIME);

  if (gameOver_) {
    DrawText(
      "GAME OVER", GetScreenWidth() / 2 - 80, GetScreenHeight() / 2, 28, RED);
    DrawText(
      "Press R to restart", GetScreenWidth() / 2 - 100,
      GetScreenHeight() / 2 + 40, 20, WHITE);
  }

  if (levelComplete_) {
    if (currentLevel_ >= maxLevels_) {
      DrawText(
        "CONGRATULATIONS!", GetScreenWidth() / 2 - 120,
        GetScreenHeight() / 2 - 20, 28, GOLD);
      DrawText(
        "YOU COMPLETED ALL LEVELS!", GetScreenWidth() / 2 - 150,
        GetScreenHeight() / 2 + 20, 20, GOLD);
      DrawText(
        "Press R to restart", GetScreenWidth() / 2 - 100,
        GetScreenHeight() / 2 + 60, 20, WHITE);
    } else {
      DrawText(
        TextFormat("LEVEL %d COMPLETE!", currentLevel_),
        GetScreenWidth() / 2 - 120, GetScreenHeight() / 2, 28, GREEN);
      DrawText(
        "Press SPACE for next level", GetScreenWidth() / 2 - 120,
        GetScreenHeight() / 2 + 40, 20, WHITE);
    }
  }
}

void GameManaging::DrawBackground() {
  // Draw background based on level temporary
  Color bgColor = SKYBLUE;
  switch (currentLevel_) {
    case 1:
      bgColor = SKYBLUE;
      break;
    case 2:
      bgColor = {135, 206, 235, 255};
      break;
    case 3:
      bgColor = {25, 25, 112, 255};
      break;
  }
  ClearBackground(bgColor);
}

void GameManaging::UpdateTime() {
  float delta = GetFrameTime();
  gameTime_  += delta;
  if (countdownSeconds_ - (int)gameTime_ <= 0) {
    DecreaseLife();
    gameTime_ = 0;
  }
}

void GameManaging::AddPoints(int points) {
  points_ += points;
}

void GameManaging::DecreaseLife() {
  lives_--;
  if (lives_ <= 0) {
    gameOver_ = true;
  }
}

void GameManaging::HitBlock(GameObject *block, Character *character) {
  if (block) {
    block->OnHit();
    AddPoints(50);
    Rectangle blockRect = block->GetRectangle();
    SpawnParticle({blockRect.x + 16, blockRect.y}, YELLOW);
    PlaySoundEffect("block_hit");
  }
  (void)character;
}

void
GameManaging::SpawnParticle(Vector2 position, Color color, Vector2 velocity) {
  Particle particle;
  particle.position = position;
  particle.velocity = velocity;
  particle.color    = color;
  particle.life     = 1.0f;
  particle.maxLife  = 1.0f;
  particles_.push_back(particle);
}

void GameManaging::CleanupDeadParticles() {
  particles_.erase(
    std::remove_if(
      particles_.begin(), particles_.end(),
      [](const Particle &p) {
        return p.life <= 0;
      }),
    particles_.end());
}

void GameManaging::CreateEnemyFromType(int enemyType, Vector2 position) {
  EnemyManager &enemyManager = EnemyManager::GetInstance();

  switch (enemyType) {
    case 90: // Goomba
      enemyManager.SpawnEnemy(EnemyType::Goomba, position);
      break;
    case 95: // Piranha
      enemyManager.SpawnEnemy(EnemyType::Piranha, position);
      break;
    case 99: // Koopa
      enemyManager.SpawnEnemy(EnemyType::Koopa, position);
      break;
    case 100: // Bowser (boss enemy for level 3)
      enemyManager.SpawnEnemy(EnemyType::Bowser, position);
      break;
    default:
      std::cout << "Unknown enemy type: " << enemyType << std::endl;
      break;
  }
  // Note: Enemy is automatically added to collision system in
  // EnemyManager::AddEnemy()
}

void GameManaging::CreateBlockFromType(int tileType, Vector2 position) {
  ObjectManager &objectManager = ObjectManager::GetInstance();

  switch (tileType) {
    case 1: // Brick block
      objectManager.AddBrickBlock(position);
      break;
    case 2: // Question block with coin
      objectManager.AddQuestionBlock(position, QuestionBlockItem::coin);
      break;
    case 3: // Question block with power-up
      objectManager.AddQuestionBlock(
        position, QuestionBlockItem::super_mushroom);
      break;
    case 4: // Static ground block
      objectManager.AddStaticBlockByTheme(position, 'g');
      break;
    case 5: // Pipe block
      objectManager.AddStaticBlockByTheme(position, 'p');
      break;
    default:
      // Default to ground block for unknown types
      objectManager.AddStaticBlockByTheme(position, 'g');
      break;
  }
  return;
}

void
GameManaging::CreateSpecialObjectFromType(int specialType, Vector2 position) {
  switch (specialType) {
    case 200: // Spawn point
      spawnPoint_ = position;
      std::cout << "Spawn point set to: " << position.x << ", " << position.y
                << std::endl;
      break;
    case 201: // Level end flag
      levelEndX_ = position.x;
      std::cout << "Level end set to: " << position.x << std::endl;
      break;
    case 202: // Checkpoint
      // Could implement checkpoints for longer levels
      break;
    default:
      break;
  }
}

void GameManaging::UpdateParticles(float deltaTime) {
  for (auto &particle : particles_) {
    particle.position.x += particle.velocity.x * deltaTime;
    particle.position.y += particle.velocity.y * deltaTime;
    particle.velocity.y += 200.0f * deltaTime; // Gravity
    particle.life       -= deltaTime;

    // Fade out
    float alpha      = particle.life / particle.maxLife;
    particle.color.a = (unsigned char)(255 * alpha);
  }
}

void GameManaging::PlaySoundEffect(const std::string &soundName) {
  (void)soundName; // Audio placeholder
}

void GameManaging::UpdateBackgroundMusic() {
  // Background music placeholder - could change music based on level
}

void GameManaging::LoadNextLevel() {
  if (currentLevel_ >= maxLevels_) {
    // All levels completed - could restart or show credits
    std::cout << "All levels completed! Restarting game..." << std::endl;
    ResetGame();
    return;
  }

  currentLevel_++;
  levelComplete_ = false;

  // Try to load the next level from JSON file
  std::string levelFile
    = "res/levels/level" + std::to_string(currentLevel_) + ".json";

  std::cout << "Attempting to load next level: " << levelFile << std::endl;
  LoadLevel(levelFile);

  // Reset character position to spawn point
  if (sceneCamera_) {
    sceneCamera_->target = spawnPoint_;
  }
}

void GameManaging::ResumeEnemies() {
  EnemyManager::GetInstance().ResumeAllEnemies();
}

bool GameManaging::IsLevelComplete() const {
  return levelComplete_;
}

bool GameManaging::IsGameOver() const {
  return gameOver_;
}

int GameManaging::GetCurrentLevel() const {
  return currentLevel_;
}

int GameManaging::GetMaxLevels() const {
  return maxLevels_;
}

bool GameManaging::CanAdvanceLevel() const {
  return levelComplete_ && currentLevel_ < maxLevels_;
}

void GameManaging::HandleLevelProgression() {
  if (levelComplete_) {
    if (IsKeyPressed(KEY_SPACE) && currentLevel_ < maxLevels_) {
      LoadNextLevel();
    } else if (IsKeyPressed(KEY_R) && currentLevel_ >= maxLevels_) {
      ResetGame();
    }
  }

  if (gameOver_ && IsKeyPressed(KEY_R)) {
    ResetGame();
  }
}
