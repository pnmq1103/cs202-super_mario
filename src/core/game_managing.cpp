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
#include "include/core/game.hpp"
#include "include/core/game_info.hpp"
#include "include/core/map.hpp"
#include "include/external/json.hpp"
#include "include/managers/enemy_manager.hpp"
#include "include/objects/object_manager.hpp"

using json = nlohmann::json;

GameManaging::GameManaging() {
  // Basic game state initialization
  lives_            = GameInfo::GetInstance().life;
  points_           = GameInfo::GetInstance().coin;
  gameTime_         = 0.0f;
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
  levelEndX_ = constants::mapWidth * constants::blockSize
               - 75.0f; // Character must reach this X
                        // position to complete level
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
    EnemyManager::GetInstance().SetCollisionHandler(collisionHandler_);
  }
}

void
GameManaging::InitializeCollisionSystem(float worldWidth, float worldHeight) {
  levelWidth_  = worldWidth;
  levelHeight_ = worldHeight;
}

void GameManaging::RegisterCharacterWithCollision(Character *character) {
  if (collisionHandler_ && character) {
    collisionHandler_->AddCharacter(character);
  }
}

// Lac lam
void GameManaging::LoadLevel(const std::string &filename) {
  std::cout << "Loading level: " << filename << std::endl;

  // If first time loading this level, set initial points
  // if (isFirstTimeInLevel(currentLevel_)) {
  //  // For level 1, always start with 0 or loaded points
  //  if (currentLevel_ == 1) {
  //    initialLevelPoints_[currentLevel_] = 0;
  //    points_                            = 0;but
  //    GameInfo::GetInstance().coin       = 0;
  //  }
  //  // For other levels, use current accumulated points
  //  else {
  //    initialLevelPoints_[currentLevel_] = GameInfo::GetInstance().coin;
  //    points_                            = GameInfo::GetInstance().coin;
  //  }

  //  std::cout << "Initial points for level " << currentLevel_
  //            << " set to: " << initialLevelPoints_[currentLevel_] <<
  //            std::endl;
  //}
  //// If restarting this level (after death), reset to initial points
  // else {
  //   points_                      = initialLevelPoints_[currentLevel_];
  //   GameInfo::GetInstance().coin = points_;
  //   std::cout << "Restarting level " << currentLevel_
  //             << " with points: " << points_ << std::endl;
  // }

  UnloadLevel();

  // IMPORTANT: Make sure collision handler is properly initialized BEFORE Reset
  // Reset managers
  // Reset ObjectManager with the VALID collision handler
  // Reset level completion tracking
  levelComplete_ = false;

  // Try to load from JSON file
  try {
    Map map;
    FileHandler::LoadMapFromFile(map, filename);

    // Process map data to create game objects
    int mapSize = constants::mapWidth * constants::mapHeight;
    for (int i = 0; i < mapSize; i++) {
      // get the gid
      int tileGid        = map.GetCell(MapLayer::Tile1, i);
      int enemyGid       = map.GetCell(MapLayer::Objects, i);
      int pipeGid        = map.GetCell(MapLayer::Tile2, i);
      Rectangle tileRect = {0, 0, 0, 0}; // Initialize tile rectangle
      // draw tile1
      if (tileGid != 0) {
        tileRect             = map.GetInfo(tileGid);
        Vector2 tilePosition = {
          (float)(i % constants::mapWidth) * constants::blockSize,
          (float)(i / constants::mapWidth) * constants::blockSize};
        CreateBlockFromType(tileGid, tilePosition, tileRect);
      }

      // draw objects
      if (enemyGid != 0) {
        Vector2 enemyPosition = {
          (float)(i % constants::mapWidth) * constants::blockSize,
          (float)(i / constants::mapWidth) * constants::blockSize};
        CreateEnemyFromType(enemyGid, enemyPosition);
      }

      // draw the pipes last so they are on top of the piranha plants
      if (pipeGid != 0) {
        Vector2 pipePosition = {
          (float)(i % constants::mapWidth) * constants::blockSize,
          (float)(i / constants::mapWidth) * constants::blockSize};
        // hard-code pipe blocks; all pipes in map will be the same size
        ObjectManager::GetInstance().AddPipeBlock(
          pipePosition, constants::blockSize * 3, true, true, false);
        std::cout << "Added pipe block at " << pipePosition.x << ", "
                  << pipePosition.y << std::endl;
      }
    }

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
      countdownSeconds_ = 200;
      break;
    case 2:
      countdownSeconds_ = 250;
      break;
    case 3:
      countdownSeconds_ = 200;
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
}

void GameManaging::ResetGame() {
  lives_                        = 3;
  GameInfo::GetInstance().life  = lives_;
  points_                       = 0;
  GameInfo ::GetInstance().coin = 0;
  gameTime_                     = 0.0f;
  gameOver_                     = false;
  levelComplete_                = false;
  currentLevel_                 = 1;
  spawnPoint_                   = {100.0f, 500.0f};
  isDeathHandled_               = false;
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

  // Clear dead enemies (killed by boundaries or other means)
  enemyManager.ClearDeadEnemies();

  CheckLevelCompletion(activeCharacter);

  // Update particles
  UpdateParticles(deltaTime);
  CleanupDeadParticles();

  // Update background music
  UpdateBackgroundMusic();

  // Handle character death
  if (activeCharacter->IsDead() && !isDeathHandled_) {
    OnPlayerDeath(character_);
    isDeathHandled_ = true; // ensure we only handle death once
  }
}

void GameManaging::CheckLevelCompletion(Character *activeCharacter) {
  if (!activeCharacter || levelComplete_)
    return;

  Rectangle charRect = activeCharacter->GetRectangle();

  // Check if character reached the end of the level
  bool reachedEnd = charRect.x >= levelEndX_;

  if (reachedEnd && EnemyManager::GetInstance().IsBossClear()) {
    levelComplete_    = true;
    int timeUsed      = (int)gameTime_;
    int timeRemaining = countdownSeconds_ - timeUsed;

    GameInfo::GetInstance().SetLevelCompletionTime(currentLevel_, timeUsed);

    AddPoints(1000 * currentLevel_); // Bonus points for completing level
    GameInfo::GetInstance().coin = points_;
    FileHandler::SaveGameInfo(GameInfo::GetInstance());

    std::cout << "Level " << currentLevel_ << " completed by reaching the end!"
              << std::endl;
    App.Media().PlaySound("flagpole"); // Flagpole completion sound
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
  ObjectManager::GetInstance().Draw();
  // Draw enemies using EnemyManager
  EnemyManager &enemyManager = EnemyManager::GetInstance();
  for (Enemy *enemy : enemyManager.GetEnemies()) {
    if (enemy && (enemy->IsAlive() || enemy->IsRunningDeathAnimation())) {
      enemy->Draw();
    }
  }
  ObjectManager::GetInstance().Draw();

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
  if (!sceneCamera_)
    return;

  // Calculate camera-relative position for HUI
  Vector2 camera_offset = {
    sceneCamera_->target.x - sceneCamera_->offset.x / sceneCamera_->zoom,
    sceneCamera_->target.y - sceneCamera_->offset.y / sceneCamera_->zoom};

  // HUI positioning (top-left corner relative to camera)
  float hui_start_x   = camera_offset.x + 20.0f;
  float hui_start_y   = camera_offset.y + 20.0f;
  float hui_spacing_y = 40.0f;
  float sprite_size   = 32.0f;
  float text_offset_x = sprite_size + 10.0f;

  // Try to load HUI textures if not already loaded (static to avoid reloading)
  static Texture coin_hui_texture  = {0};
  static Texture lives_hui_texture = {0};
  static Texture time_hui_texture  = {0};
  static bool textures_loaded      = false;

  if (!textures_loaded) {
    coin_hui_texture  = LoadTexture("res/ui/coin.png");
    lives_hui_texture = LoadTexture("res/ui/lives.png");
    time_hui_texture  = LoadTexture("res/ui/time.png");
    textures_loaded   = true;
  }

  // Draw coin HUI and points
  if (IsTextureValid(coin_hui_texture)) {
    Rectangle coin_dest = {hui_start_x, hui_start_y, sprite_size, sprite_size};
    DrawTexturePro(
      coin_hui_texture,
      {0, 0, (float)coin_hui_texture.width, (float)coin_hui_texture.height},
      coin_dest, {0, 0}, 0.0f, WHITE);
  }
  DrawText(
    TextFormat("%d", points_), (int)(hui_start_x + text_offset_x),
    (int)hui_start_y + 5, 24, YELLOW);

  // Draw lives HUI and lives count
  float lives_y = hui_start_y + hui_spacing_y;
  if (IsTextureValid(lives_hui_texture)) {
    Rectangle lives_dest = {hui_start_x, lives_y, sprite_size, sprite_size};
    DrawTexturePro(
      lives_hui_texture,
      {0, 0, (float)lives_hui_texture.width, (float)lives_hui_texture.height},
      lives_dest, {0, 0}, 0.0f, WHITE);
  }
  DrawText(
    TextFormat("%d", lives_), (int)(hui_start_x + text_offset_x),
    (int)lives_y + 5, 24, WHITE);

  // Draw time HUI and countdown
  float time_y = hui_start_y + hui_spacing_y * 2;
  if (IsTextureValid(time_hui_texture)) {
    Rectangle time_dest = {hui_start_x, time_y, sprite_size, sprite_size};
    DrawTexturePro(
      time_hui_texture,
      {0, 0, (float)time_hui_texture.width, (float)time_hui_texture.height},
      time_dest, {0, 0}, 0.0f, WHITE);
  }
  int remaining_time = countdownSeconds_ - (int)gameTime_;
  DrawText(
    TextFormat("%d", remaining_time), (int)(hui_start_x + text_offset_x),
    (int)time_y + 5, 24, GREEN);

  // Additional game info (level and enemies) - positioned below HUI
  float info_y = hui_start_y + hui_spacing_y * 3;
  DrawText(
    TextFormat("Level: %d/%d", currentLevel_, maxLevels_), (int)hui_start_x,
    (int)info_y, 20, BLUE);

  // Game over and level complete messages (centered on screen)
  if (gameOver_) {
    Vector2 center = {
      camera_offset.x + GetScreenWidth() / (2.0f * sceneCamera_->zoom),
      camera_offset.y + GetScreenHeight() / (2.0f * sceneCamera_->zoom)};
    DrawText("GAME OVER", (int)center.x - 80, (int)center.y, 28, RED);
    DrawText(
      "Press Space to restart", (int)center.x - 100, (int)center.y + 40, 20,
      WHITE);
  }

  if (levelComplete_) {
    Vector2 center = {
      camera_offset.x + GetScreenWidth() / (2.0f * sceneCamera_->zoom),
      camera_offset.y + GetScreenHeight() / (2.0f * sceneCamera_->zoom)};
    if (currentLevel_ == maxLevels_) {
      DrawText(
        "CONGRATULATIONS!", (int)center.x - 275, (int)center.y - 80, 60,
        RAYWHITE);
      DrawText(
        "YOU COMPLETED ALL LEVELS!", (int)center.x - 200, (int)center.y - 20,
        30, RAYWHITE);
      DrawText(
        "Press Space to restart", (int)center.x - 175, (int)center.y + 70, 30,
        RED);
    } else {
      DrawText(
        TextFormat("LEVEL %d COMPLETE!", currentLevel_), (int)center.x - 150,
        (int)center.y + 40, 30, RED);
      DrawText(
        "Press Space to continue", (int)center.x - 175, (int)center.y + 70, 30,
        RED);
    }
  }
}

void GameManaging::DrawBackground() {
  float x = 0;
  for (int i = 0; i < 10; i++) {
    DrawTexture(App.Resource().GetBackground(currentLevel_), x, 0, RAYWHITE);
    x += 2048.0f;
  }
}

void GameManaging::UpdateTime() {
  float delta = GetFrameTime();
  gameTime_  += delta;

  int remainingTime = countdownSeconds_ - (int)gameTime_;

  // Play warning sound when time is running low
  static bool timeWarningPlayed = false;
  if (remainingTime <= 100 && remainingTime > 0 && !timeWarningPlayed) {
    App.Media().PlaySound("time-up_warning");
    timeWarningPlayed = true;
  }

  // Reset warning flag when time is reset or level changes
  if (remainingTime > 100) {
    timeWarningPlayed = false;
  }

  if (remainingTime <= 0) {
    App.Media().PlaySound("life_lost");
    DecreaseLife();
    gameTime_ = 0;
  }
}

void GameManaging::AddPoints(int points) {
  points_                     += points;
  GameInfo::GetInstance().coin = points_;

  // Play coin sound for small point gains (like collecting coins)
  if (points <= 200) {
    App.Media().PlaySound("coin");
  }

  // Check for 1-up (extra life) milestone
  static int lastLifeThreshold = 0;
  int currentLifeThreshold = points_ / 10000; // Every 10,000 points = 1 life

  if (currentLifeThreshold > lastLifeThreshold) {
    lives_++;
    App.Media().PlaySound("1up");
    lastLifeThreshold = currentLifeThreshold;
    std::cout << "Extra life earned! Lives: " << lives_ << std::endl;
  }
}

void GameManaging::DecreaseLife() {
  lives_--;
  // Make sure GameInfo is also synchronized
  GameInfo::GetInstance().life = lives_;
}

void GameManaging::HitBlock(GameObject *block, Character *character) {
  if (block) {
    block->OnHit();
    AddPoints(50);
    Rectangle blockRect = block->GetRectangle();
    SpawnParticle({blockRect.x + 16, blockRect.y}, YELLOW);

    // Play appropriate sound based on block type
    if (block->GetType() == ObjectType::BrickBlock) {
      App.Media().PlaySound("brick");
    } else if (block->GetType() == ObjectType::Block) {
      App.Media().PlaySound("powerup");
    } else {
      App.Media().PlaySound("bump");
    }
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
    case 80: // Goomba
      enemyManager.SpawnEnemy(EnemyType::Goomba, position);
      break;
    case 81: // Piranha
      enemyManager.SpawnEnemy(EnemyType::Piranha, position);
      break;
    case 82: // Koopa
      enemyManager.SpawnEnemy(EnemyType::Koopa, position);
      break;
    case 79: // Bowser (boss enemy for level 3)
      enemyManager.SpawnBoss(position, character_);
      break;
    default:
      std::cout << "Unknown enemy type: " << enemyType << std::endl;
      break;
  }
  // Note: Enemy is automatically added to collision system in
  // EnemyManager::AddEnemy()
}

void GameManaging::CreateBlockFromType(
  int tileGid, Vector2 tilePosition, Rectangle tileRect) {
  if (tileGid == 1) {
    ObjectManager::GetInstance().AddBrickBlock(tilePosition);
  } else if (tileGid == 2) {
    ObjectManager::GetInstance().AddQuestionBlock(
      tilePosition, std::rand() % 4);
  } else if (tileGid == 3) {
    ObjectManager::GetInstance().AddCoin(tilePosition);
  } else if (tileGid >= 17 && tileGid <= 37) { // using the minimal tileset
    ObjectManager::GetInstance().AddStaticBlockByGid(
      tilePosition, tileRect); // add by gid instead of theme's default block
  } else {
    // fallback for debugging
    std::cout << "Unknown tile GID: " << tileGid << std::endl;
    ObjectManager::GetInstance().AddStaticBlockByTheme(
      tilePosition, 'g'); // default to ground block
  }
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
  App.Media().PlaySound(soundName);
}

void GameManaging::UpdateBackgroundMusic() {
  App.Media().UpdateMusic();

  // Handle special game state music
  static bool gameOverSoundPlayed      = false;
  static bool levelCompleteSoundPlayed = false;

  if (gameOver_ && !gameOverSoundPlayed) {
    App.Media().PlaySound("gameover");
    App.Media().StopMusic(); // Stop background music when game over
    gameOverSoundPlayed = true;
  } else if (!gameOver_) {
    gameOverSoundPlayed = false; // Reset flag when game is not over
  }

  if (levelComplete_ && !levelCompleteSoundPlayed) {
    App.Media().PlaySound("level_complete");
    levelCompleteSoundPlayed = true;
  } else if (!levelComplete_) {
    levelCompleteSoundPlayed = false; // Reset flag when level is not complete
  }
}

void GameManaging::LoadNextLevel() {
  if (currentLevel_ >= maxLevels_) {
    std::cout << "All levels completed! Restarting game..." << std::endl;
    ResetGame();
    return;
  }

  currentLevel_++;
  levelComplete_ = false;

  // Try to load the next level from JSON file
  std::string levelFile
    = "res/maps/map" + std::to_string(currentLevel_) + ".json";

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

// void GameManaging::RestartCurrentLevel(Character *character) {
//   // App.RemoveScene();
//   // App.AddScene(
//   //
//   std::make_unique<GameScene>(CharacterSelectorScene::GetCharacterType()));
//   if (GameInfo::GetInstance().life <= 0) {
//     // Game over - reset everything and go to level 1
//     ResetGame();
//     LoadLevel("res/maps/map1.json");
//   } else {
//     // Restart current level - we have lives left
//     // Reset character position to spawn point
//     if (character) {
//       character->SetCharacter(character->GetCharacter(), spawnPoint_);
//     }
//
//     // Reset level state but keep GameInfo data (lives, coins, etc.)
//     // You might need to reload the current level JSON here
//
//     isDeathHandled_ = false;
//     gameOver_       = false;
//   }
// }
void GameManaging::OnPlayerDeath(Character *character) {
  if (character) {
    // Play death sound
    App.Media().PlaySound("life_lost");
  }
}
void GameManaging::SetLives(int lives) {
  lives_ = lives;
}
void GameManaging::SetCurrentLevel(int level) {
  currentLevel_ = level;
  // Select different music based on level
  switch (currentLevel_) {
    case 1:
      App.Media().PlayMusic("ground_theme");
      break;
    case 2:
      App.Media().PlayMusic("underground_theme");
      break;
    case 3:
      App.Media().PlayMusic("castle_theme");
      break;
    default:
      App.Media().PlayMusic("ground_theme");
      break;
  }
}