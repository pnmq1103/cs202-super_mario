#include "include/core/game_managing.hpp"
#include <algorithm>
#include <cmath>
#include <raylib.h>
#include <stdexcept>

#include "include/characters/character.hpp"
#include "include/core/application.hpp"
#include "include/core/constants.hpp"
#include "include/core/map.hpp"
#include "include/core/resource_manager.hpp"
#include "include/enemies/bowser.hpp"
#include "include/enemies/goomba.hpp"
#include "include/enemies/koopa_troopa.hpp"
#include "include/enemies/piranha_plant.hpp"
#include "include/external/json.hpp"
#include "include/managers/enemy_manager.hpp"
#include "include/objects/object_manager.hpp"

using json = nlohmann::json;

GameManaging::GameManaging() {
  lives_ = 3;
  gameTime_ = 0.0f;
  points_ = 0;
  countdownSeconds_ = 300;
  backgroundType_ = 0;
  currentLevel_ = 1;
  levelComplete_ = false;
  gameOver_ = false;
  spawnPoint_ = {100.0f, 500.0f};
  enemyManager_ = std::make_unique<EnemyManager>();
  updateCounter_ = 0;

  // Camera initialization
  camera_.target = {100.0f, 300.0f};
  camera_.offset = {GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};
  camera_.rotation = 0.0f;
  camera_.zoom = 1.0f;
  cameraTarget_ = {100.0f, 300.0f};
  cameraOffsetX_ = 0.0f;
  cameraOffsetY_ = 0.0f;

  // Level boundaries
  levelWidth_ = 3200.0f;
  levelHeight_ = 800.0f;
  groundLevel_ = 600.0f;

  // Initialize collision system with default world size
  InitializeCollisionSystem(levelWidth_, levelHeight_);

  LoadResources();
}

GameManaging::~GameManaging() {
  UnloadLevel();
}

void GameManaging::InitializeCollisionSystem(float worldWidth, float worldHeight) {
  // Create the collision handler with proper world dimensions
  collisionHandler_ = std::make_unique<CollisionHandler>(worldWidth, worldHeight);
}

void GameManaging::RegisterCharacterWithCollision(Character *character) {
  if (collisionHandler_ && character) {
    collisionHandler_->AddCharacter(character);
  }
}

void GameManaging::RegisterEnemyWithCollision(Enemy *enemy) {
  if (collisionHandler_ && enemy) {
    collisionHandler_->AddEnemy(enemy);
  }
}

void GameManaging::UpdateCollisionSystem() {
  if (collisionHandler_) {
    // Register all current blocks with collision system
    for (size_t i = 0; i < blocks_.size(); ++i) {
      if (blocks_[i] && !blocks_[i]->IsDestroyed()) {
        collisionHandler_->AddObject(blocks_[i].get());
      }
    }

    // Register all enemies with collision system
    if (enemyManager_) {
      for (Enemy* enemy : enemyManager_->GetEnemies()) {
        if (enemy && enemy->IsAlive()) {
          collisionHandler_->AddEnemy(enemy);
        }
      }
    }

    // Run collision detection
    collisionHandler_->CheckCollision();
  }
}

void GameManaging::LoadLevel(const std::string &filename) {
  try {
    UnloadLevel();

    // Reset collision system for new level
    if (collisionHandler_) {
      collisionHandler_->Reset(levelWidth_, levelHeight_);
    }

    // Try to load from file first
    std::ifstream file(filename);
    if (file.is_open()) {
      Map map;
      json j;
      try {
        file >> j;
        j.get_to(map);

        int mapSize = constants::mapWidth * constants::mapHeight;
        for (int i = 0; i < mapSize; i++) {
          // Get tile and create block
          int tileGid = map.GetTile(i);
          Vector2 tilePosition = {
            (float)(i % constants::mapWidth) * 16.0f,
            (float)(i / constants::mapHeight) * 16.0f};
          CreateBlockFromType(tileGid, tilePosition);

          // Get enemy and create enemy
          int enemyGid = map.GetEnemy(i);
          Vector2 enemyPosition = {
            (float)(i % constants::mapWidth) * 16.0f,
            (float)(i / constants::mapHeight) * 16.0f};
          CreateEnemyFromType(enemyGid, enemyPosition);
        }
      } catch (const json::exception &e) {
        std::cerr << "JSON parsing error: " << e.what() << std::endl;
        // Fall back to creating default level
        CreateLevel1();
      }
    } else {
      // File doesn't exist or can't be opened, create default level
      std::cout << "Level file not found, creating default level..." << std::endl;
      CreateLevel1();
    }

    // Set camera target to spawn point
    camera_.target = spawnPoint_;
    cameraTarget_ = spawnPoint_;
    
  } catch (const std::exception &e) {
    std::cerr << "Error loading level: " << e.what() << std::endl;
    // Fall back to creating a basic level
    CreateLevel1();
  }
}

void GameManaging::CreateLevel1() {
  // Ground terrain
  CreatePlatform({0, groundLevel_}, {levelWidth_, groundLevel_}, 1);

  // Question blocks with power-ups
  CreateBlockFromType(2, {300.0f, groundLevel_ - 150.0f});
  CreateBlockFromType(2, {500.0f, groundLevel_ - 200.0f});
  CreateBlockFromType(2, {800.0f, groundLevel_ - 150.0f});

  // Brick blocks
  CreateBlockFromType(1, {350.0f, groundLevel_ - 150.0f});
  CreateBlockFromType(1, {400.0f, groundLevel_ - 150.0f});
  CreateBlockFromType(1, {450.0f, groundLevel_ - 150.0f});

  // Floating platforms
  CreatePlatform({600.0f, groundLevel_ - 250.0f}, {750.0f, groundLevel_ - 250.0f}, 1);
  CreatePlatform({900.0f, groundLevel_ - 300.0f}, {1050.0f, groundLevel_ - 300.0f}, 1);

  // Spawn enemies for Level 1
  SpawnLevelEnemies(1);

  backgroundType_ = 0; // Overworld
}

void GameManaging::CreateLevel2() {
  // Underground level
  CreatePlatform({0, groundLevel_}, {levelWidth_, groundLevel_}, 1);

  // More complex platform layout
  CreatePlatform({200.0f, groundLevel_ - 100.0f}, {400.0f, groundLevel_ - 100.0f}, 1);
  CreatePlatform({600.0f, groundLevel_ - 200.0f}, {800.0f, groundLevel_ - 200.0f}, 1);

  SpawnLevelEnemies(2);
  backgroundType_ = 1; // Underground
}

void GameManaging::CreateLevel3() {
  // More challenging level
  CreatePlatform({0, groundLevel_}, {levelWidth_, groundLevel_}, 1);

  // Moving platform simulation with gaps
  CreatePlatform({200.0f, groundLevel_ - 100.0f}, {350.0f, groundLevel_ - 100.0f}, 1);
  CreatePlatform({450.0f, groundLevel_ - 150.0f}, {600.0f, groundLevel_ - 150.0f}, 1);

  SpawnLevelEnemies(3);
  backgroundType_ = 0; // Overworld
}

void GameManaging::CreateBossLevel() {
  // Boss arena
  CreatePlatform({0, groundLevel_}, {1200.0f, groundLevel_}, 1);

  // Spawn boss
  SpawnBoss({600.0f, groundLevel_ - 50.0f});

  backgroundType_ = 2; // Castle
}

void GameManaging::SpawnLevelEnemies(int levelNumber) {
  if (!enemyManager_) return;

  switch (levelNumber) {
    case 1:
      // Level 1: Basic enemies
      enemyManager_->SpawnEnemy(EnemyType::Goomba, {400.0f, groundLevel_ - 20.0f});
      enemyManager_->SpawnEnemy(EnemyType::Goomba, {600.0f, groundLevel_ - 20.0f});
      enemyManager_->SpawnEnemy(EnemyType::Koopa, {800.0f, groundLevel_ - 30.0f});
      break;

    case 2:
      // Level 2: More enemies
      for (int i = 0; i < 3; i++) {
        enemyManager_->SpawnEnemy(EnemyType::Goomba, {300.0f + i * 150.0f, groundLevel_ - 20.0f});
      }
      enemyManager_->SpawnEnemy(EnemyType::Koopa, {700.0f, groundLevel_ - 30.0f});
      break;

    case 3:
      // Level 3: Challenging enemy placement
      enemyManager_->SpawnEnemy(EnemyType::Koopa, {250.0f, groundLevel_ - 120.0f});
      enemyManager_->SpawnEnemy(EnemyType::Goomba, {500.0f, groundLevel_ - 170.0f});
      break;
  }
}

void GameManaging::Update(float deltaTime, Character *activeCharacter) {
  updateCounter_++;

  // Update time
  UpdateTime();

  // Update camera to follow character
  if (activeCharacter) {
    UpdateCamera(activeCharacter);
  }

  // Update all blocks
  for (auto &block : blocks_) {
    if (block && !block->IsDestroyed()) {
      block->Update();
    }
  }

  // Update physics and particles
  UpdatePhysics(deltaTime);
  UpdateParticles(deltaTime);

  // Enhanced enemy management
  if (enemyManager_) {
    // Set character references for enemy AI
    if (activeCharacter) {
      Rectangle charRect = activeCharacter->GetRectangle();
      Vector2 charPos = {charRect.x, charRect.y};
      static Vector2 characterPosition;
      characterPosition = charPos;
      enemyManager_->SetCharacterReferences(
        &characterPosition, &characterPosition, &characterPosition);
    }

    // Update all enemies
    enemyManager_->UpdateAll(deltaTime);
    HandleEnemyAI(deltaTime);

    // Handle character interactions through enemy manager
    if (activeCharacter) {
      enemyManager_->HandleCharacterInteractions(activeCharacter);
    }

    // Handle wall collisions
    if (updateCounter_ % 2 == 0) {
      enemyManager_->HandleWallCollisions();
    }

    // Clean up dead enemies
    if (updateCounter_ % 10 == 0) {
      enemyManager_->ClearDeadEnemies();
    }
  }

  // Use the existing collision system instead of our own
  UpdateCollisionSystem();

  // Clean up dead enemies and particles less frequently
  if (updateCounter_ % 20 == 0) {
    RemoveDeadEnemies();
    CleanupDeadParticles();
  }

  // Check level completion
  CheckLevelComplete();

  // Update background music
  UpdateBackgroundMusic();
}

void GameManaging::UpdateCamera(Character *character) {
  if (!character) return;

  Rectangle charRect = character->GetRectangle();
  Vector2 charPos = {charRect.x + charRect.width / 2, charRect.y + charRect.height / 2};

  // Smooth camera following
  float smoothing = 0.08f; // Slightly faster for better responsiveness
  cameraTarget_.x = charPos.x;
  cameraTarget_.y = charPos.y - 100.0f; // Offset camera up a bit

  camera_.target.x += (cameraTarget_.x - camera_.target.x) * smoothing;
  camera_.target.y += (cameraTarget_.y - camera_.target.y) * smoothing;

  // Keep camera within level bounds
  float halfScreenWidth = GetScreenWidth() / 2.0f / camera_.zoom;
  float halfScreenHeight = GetScreenHeight() / 2.0f / camera_.zoom;

  camera_.target.x = fmaxf(halfScreenWidth, fminf(camera_.target.x, levelWidth_ - halfScreenWidth));
  camera_.target.y = fmaxf(halfScreenHeight, fminf(camera_.target.y, levelHeight_ - halfScreenHeight));
}

void GameManaging::UpdatePhysics(float deltaTime) {
  // Enhanced physics updates - ground collision for enemies
  if (enemyManager_) {
    for (Enemy* enemy : enemyManager_->GetEnemies()) {
      if (enemy && enemy->IsAlive()) {
        Rectangle enemyRect = enemy->GetRect();
        // Simple ground collision
        if (enemyRect.y + enemyRect.height >= groundLevel_) {
          Vector2 pos = enemy->GetPosition();
          pos.y = groundLevel_ - enemyRect.height;
          enemy->SetPosition(pos);
          
          Vector2 vel = enemy->GetVelocity();
          vel.y = 0.0f;
          enemy->SetVelocity(vel);
        }
      }
    }
  }
}

void GameManaging::UpdateParticles(float deltaTime) {
  // Update particle system
  for (auto &particle : particles_) {
    particle.position.x += particle.velocity.x * deltaTime;
    particle.position.y += particle.velocity.y * deltaTime;
    particle.velocity.y += 200.0f * deltaTime; // Gravity
    particle.life -= deltaTime;

    // Fade out over time
    float alpha = particle.life / particle.maxLife;
    particle.color.a = (unsigned char)(255 * alpha);
  }
}

void GameManaging::HandleEnemyAI(float deltaTime) {
  (void)deltaTime; // Enhanced AI logic placeholder
}

void GameManaging::CheckLevelComplete() {
  // Check if level objectives are met
  if (!levelComplete_) {
    // For boss levels, check if boss is defeated
    if (currentLevel_ == 4) {
      if (IsBossDefeated()) {
        levelComplete_ = true;
        PlaySoundEffect("level_complete");
        SpawnParticle({400, 300}, GOLD);
      }
    }
  }
}

void GameManaging::DrawLevel() {
  BeginMode2D(camera_);

  DrawBackground();

  // Draw all blocks
  for (const auto &block : blocks_) {
    if (block && !block->IsDestroyed()) {
      DrawBlock(block.get());
    }
  }

  // Enhanced enemy rendering
  if (enemyManager_) {
    DrawEnemiesAdvanced();
  }

  // Draw legacy enemies
  for (const auto &enemy : enemies_) {
    if (enemy && enemy->IsAlive()) {
      DrawEnemy(enemy.get());
    }
  }

  // Draw particles
  for (const auto &particle : particles_) {
    DrawPixelV(particle.position, particle.color);
  }

  EndMode2D();

  // Draw UI elements (not affected by camera)
  DrawStats();
  DrawMiniMap();
  DrawLevelInfo();
}

void GameManaging::DrawStats() const {
  DrawText(TextFormat("Lives: %d", lives_), 10, 10, 24, WHITE);
  DrawText(TextFormat("Points: %d", points_), 10, 40, 24, YELLOW);
  DrawText(TextFormat("Time: %d", countdownSeconds_ - (int)gameTime_), 10, 70, 24, GREEN);
  DrawText(TextFormat("Level: %d", currentLevel_), 10, 100, 20, WHITE);

  // Enhanced enemy stats display
  if (enemyManager_) {
    size_t aliveEnemies = enemyManager_->GetAliveEnemyCount();
    size_t totalEnemies = enemyManager_->GetEnemyCount();
    DrawText(TextFormat("Enemies: %zu/%zu", aliveEnemies, totalEnemies), 10, 130, 16, WHITE);

    // Boss status
    if (enemyManager_->HasBoss()) {
      Enemy *boss = enemyManager_->GetBoss();
      if (boss) {
        float healthPercentage = boss->GetHealth() / boss->GetMaxHealth();
        DrawText(TextFormat("Boss HP: %.0f%%", healthPercentage * 100), 10, 150, 20, RED);
      }
    }
  }

  if (levelComplete_) {
    DrawText("LEVEL COMPLETE!", GetScreenWidth() / 2 - 100, GetScreenHeight() / 2, 24, GOLD);
  }

  if (IsGameOver()) {
    DrawText("GAME OVER", GetScreenWidth() / 2 - 80, GetScreenHeight() / 2, 28, RED);
  }
}

void GameManaging::DrawMiniMap() const {
  // Simple minimap in top-right corner
  int mapX = GetScreenWidth() - 200;
  int mapY = 10;
  int mapWidth = 180;
  int mapHeight = 60;

  DrawRectangle(mapX, mapY, mapWidth, mapHeight, ColorAlpha(BLACK, 0.5f));
  DrawRectangleLines(mapX, mapY, mapWidth, mapHeight, WHITE);

  // Draw level bounds
  float scaleX = mapWidth / levelWidth_;
  float scaleY = mapHeight / levelHeight_;

  // Draw blocks on minimap
  for (const auto &block : blocks_) {
    if (block && !block->IsDestroyed()) {
      Rectangle blockRect = const_cast<GameObject *>(block.get())->GetRectangle();
      int miniX = mapX + (int)(blockRect.x * scaleX);
      int miniY = mapY + (int)(blockRect.y * scaleY);
      DrawPixel(miniX, miniY, GRAY);
    }
  }

  // Draw camera position
  int camX = mapX + (int)(camera_.target.x * scaleX);
  int camY = mapY + (int)(camera_.target.y * scaleY);
  DrawCircle(camX, camY, 2, RED);
}

void GameManaging::DrawLevelInfo() const {
  // Draw level-specific information
  const char *levelNames[] = {"Plains", "Underground", "Sky", "Castle"};
  const char *currentLevelName = (currentLevel_ <= 4) ? levelNames[currentLevel_ - 1] : "Unknown";

  DrawText(TextFormat("Area: %s", currentLevelName), GetScreenWidth() - 150, GetScreenHeight() - 30, 16, WHITE);
}

// Helper implementations
void GameManaging::CreatePlatform(Vector2 start, Vector2 end, int blockType) {
  float blockSize = 32.0f; // Standard block size

  if (start.y == end.y) {
    // Horizontal platform
    for (float x = start.x; x <= end.x; x += blockSize) {
      CreateBlockFromType(blockType, {x, start.y});
    }
  } else {
    // Vertical platform
    for (float y = start.y; y <= end.y; y += blockSize) {
      CreateBlockFromType(blockType, {start.x, y});
    }
  }
}

void GameManaging::CreateStairs(Vector2 start, int steps, bool ascending) {
  float blockSize = 32.0f;

  for (int i = 0; i < steps; i++) {
    float x = start.x + i * blockSize;
    float y = ascending ? start.y - i * blockSize : start.y + i * blockSize;

    // Create vertical column of blocks
    for (int j = 0; j <= i; j++) {
      CreateBlockFromType(1, {x, y + j * blockSize});
    }
  }
}

void GameManaging::CreatePipe(Vector2 position, int height) {
  float blockSize = 32.0f;

  for (int i = 0; i < height; i++) {
    CreateBlockFromType(1, {position.x, position.y - i * blockSize});
    CreateBlockFromType(1, {position.x + blockSize, position.y - i * blockSize});
  }
}

void GameManaging::SpawnParticle(Vector2 position, Color color, Vector2 velocity) {
  Particle particle;
  particle.position = position;
  particle.velocity = velocity;
  particle.color = color;
  particle.life = 1.0f;
  particle.maxLife = 1.0f;
  particles_.push_back(particle);
}

void GameManaging::CleanupDeadParticles() {
  particles_.erase(
    std::remove_if(particles_.begin(), particles_.end(),
      [](const Particle &p) { return p.life <= 0; }),
    particles_.end());
}

Rectangle GameManaging::GetBlockCollisionRect(const GameObject *block) const {
  return const_cast<GameObject *>(block)->GetRectangle();
}

bool GameManaging::IsOnGround(Character *character) const {
  Rectangle charRect = character->GetRectangle();
  Rectangle groundCheck = {charRect.x, charRect.y + charRect.height, charRect.width, 5};

  for (const auto &block : blocks_) {
    if (block && !block->IsDestroyed() && CheckCollisionRecs(groundCheck, block->GetRectangle())) {
      return true;
    }
  }

  return charRect.y + charRect.height >= groundLevel_;
}

void GameManaging::HandleVerticalCollision(Character *character, const GameObject *block) {
  (void)character;
  (void)block; // Implementation placeholder
}

void GameManaging::HandleHorizontalCollision(Character *character, const GameObject *block) {
  (void)character;
  (void)block; // Implementation placeholder
}

void GameManaging::PlaySoundEffect(const std::string &soundName) {
  (void)soundName; // Audio implementation placeholder
}

void GameManaging::UpdateBackgroundMusic() {
  // Background music management placeholder
}

void GameManaging::DrawBackground() {
  switch (backgroundType_) {
    case 0: // Overworld
      ClearBackground(SKYBLUE);
      break;
    case 1: // Underground
      ClearBackground(BLACK);
      break;
    case 2: // Castle (for boss fights)
      ClearBackground(DARKGRAY);
      break;
    default:
      ClearBackground(SKYBLUE);
      break;
  }
}

void GameManaging::DrawEnemiesAdvanced() {
  // Draw enemies with enhanced visual effects
  for (Enemy *enemy : enemyManager_->GetEnemies()) {
    if (enemy && enemy->IsAlive()) {
      Rectangle rect = enemy->GetRect();
      Color color = RED;

      // Color based on enemy type
      switch (enemy->GetType()) {
        case EnemyType::Goomba:
          color = BROWN;
          break;
        case EnemyType::Koopa:
          color = GREEN;
          break;
        case EnemyType::Piranha:
          color = DARKGREEN;
          break;
        case EnemyType::Bowser:
          color = ORANGE;
          rect.width *= 1.5f;
          rect.height *= 1.5f;
          break;
      }

      // Visual effects based on state
      switch (enemy->GetState()) {
        case EnemyState::Stunned:
          color = ColorAlpha(color, 0.7f);
          break;
        case EnemyState::Attacking:
          color = ColorBrightness(color, 0.3f);
          break;
        case EnemyState::Shell:
          color = GRAY;
          break;
        default:
          break;
      }

      if (enemy->IsInvulnerable()) {
        color = ColorAlpha(color, 0.5f);
      }

      DrawRectangleRec(rect, color);

      // Draw type indicator
      const char *typeName = "";
      switch (enemy->GetType()) {
        case EnemyType::Goomba: typeName = "G"; break;
        case EnemyType::Koopa: typeName = "K"; break;
        case EnemyType::Piranha: typeName = "P"; break;
        case EnemyType::Bowser: typeName = "B"; break;
      }
      DrawText(typeName, (int)rect.x + 2, (int)rect.y + 2, 12, WHITE);

      // Draw state indicators
      if (enemy->GetState() == EnemyState::Stunned) {
        DrawText("!", (int)rect.x + (int)rect.width - 8, (int)rect.y - 10, 12, YELLOW);
      } else if (enemy->GetState() == EnemyState::Attacking) {
        DrawText("*", (int)rect.x + (int)rect.width - 8, (int)rect.y - 10, 12, RED);
      }

      // Draw health bar for bosses
      if (enemy->GetType() == EnemyType::Bowser) {
        float healthPercentage = enemy->GetHealth() / enemy->GetMaxHealth();
        Rectangle healthBar = {rect.x, rect.y - 20, rect.width, 6};
        DrawRectangleRec(healthBar, RED);
        healthBar.width *= healthPercentage;
        DrawRectangleRec(healthBar, GREEN);
      }
    }
  }
}

void GameManaging::DrawEnemiesSimple() {
  // Fallback simple rendering
  for (Enemy *enemy : enemyManager_->GetEnemies()) {
    if (enemy && enemy->IsAlive()) {
      Rectangle rect = enemy->GetRect();
      Color color = RED;

      switch (enemy->GetType()) {
        case EnemyType::Goomba: color = BROWN; break;
        case EnemyType::Koopa: color = GREEN; break;
        case EnemyType::Piranha: color = DARKGREEN; break;
        case EnemyType::Bowser: color = ORANGE; break;
      }

      DrawRectangleRec(rect, color);
    }
  }
}

void GameManaging::DrawBlock(const GameObject *block) {
  if (!block) return;
  const_cast<GameObject *>(block)->Draw();
}

void GameManaging::DrawEnemy(const Enemy *enemy) {
  if (!enemy) return;

  Rectangle rect = enemy->GetRect();
  Color color = RED;
  switch (enemy->GetType()) {
    case EnemyType::Goomba: color = BROWN; break;
    case EnemyType::Koopa: color = GREEN; break;
    case EnemyType::Piranha: color = DARKGREEN; break;
    case EnemyType::Bowser: color = ORANGE; break;
  }
  DrawRectangleRec(rect, color);
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

void GameManaging::SpawnEnemy(EnemyType type, Vector2 position) {
  if (enemyManager_) {
    enemyManager_->SpawnEnemy(type, position, 0);
  }
}

void GameManaging::SpawnBoss(Vector2 position) {
  backgroundType_ = 2;
  if (enemyManager_) {
    enemyManager_->SpawnBoss(EnemyType::Bowser, position);
  }
}

void GameManaging::SetEnemyDifficulty(float multiplier) {
  if (enemyManager_) {
    enemyManager_->SetDifficulty(multiplier);
  }
}

void GameManaging::PauseEnemies() {
  if (enemyManager_) {
    enemyManager_->PauseAllEnemies();
  }
}

void GameManaging::ResumeEnemies() {
  if (enemyManager_) {
    enemyManager_->ResumeAllEnemies();
  }
}

void GameManaging::ActivateBossRageMode() {
  if (enemyManager_) {
    enemyManager_->SetBossRageMode();
  }
}

void GameManaging::RemoveDeadEnemies() {
  enemies_.erase(
    std::remove_if(enemies_.begin(), enemies_.end(),
      [](const std::unique_ptr<Enemy> &enemy) { return !enemy->IsAlive(); }),
    enemies_.end());
}

void GameManaging::UpdateTime() {
  float delta = GetFrameTime();
  gameTime_ += delta;
  if (countdownSeconds_ - (int)gameTime_ <= 0) {
    DecreaseLife();
    gameTime_ = 0;
  }
}

void GameManaging::DecreaseLife() {
  lives_--;
  if (lives_ <= 0) {
    gameOver_ = true;
  }
}

void GameManaging::AddPoints(int points) {
  points_ += points;
}

void GameManaging::ResetGame() {
  lives_ = 3;
  points_ = 0;
  gameTime_ = 0.0f;
  currentLevel_ = 1;
  levelComplete_ = false;
  gameOver_ = false;
  backgroundType_ = 0;
  spawnPoint_ = {100.0f, 500.0f};
  UnloadLevel();
}

void GameManaging::UnloadLevel() {
  blocks_.clear();
  enemies_.clear();
  particles_.clear();
  if (enemyManager_) {
    enemyManager_->ClearAllEnemies();
  }
  if (collisionHandler_) {
    collisionHandler_->Reset(levelWidth_, levelHeight_);
  }
}

void GameManaging::LoadResources() {
  // Resources are automatically loaded by ResManager singleton
}

void GameManaging::CreateBlockFromType(int tileType, Vector2 position) {
  std::unique_ptr<GameObject> newBlock;

  switch (tileType) {
    case 1: // Brick block
      newBlock = std::make_unique<BrickBlock>(position, 4.0f);
      break;
    case 2: // Question block
      newBlock = std::make_unique<QuestionBlock>(position, 4.0f, QuestionBlockItem::coin);
      break;
    default:
      newBlock = std::make_unique<StaticBlock>(position, 4.0f, 'g');
      break;
  }

  if (newBlock) {
    blocks_.push_back(std::move(newBlock));
  }
}

void GameManaging::CreateEnemyFromType(int enemyType, Vector2 position) {
  switch (enemyType) {
    case 90: // Goomba
      SpawnEnemy(EnemyType::Goomba, position);
      break;
    case 95: // Piranha
      SpawnEnemy(EnemyType::Piranha, position);
      break;
    case 99: // Koopa
      SpawnEnemy(EnemyType::Koopa, position);
      break;
    default:
      break;
  }
}

bool GameManaging::IsBossDefeated() const {
  if (enemyManager_) {
    return !enemyManager_->HasBoss();
  }
  return true;
}

int GameManaging::GetBossHP() const {
  if (enemyManager_ && enemyManager_->HasBoss()) {
    Enemy *boss = enemyManager_->GetBoss();
    if (boss) {
      return (int)boss->GetHealth();
    }
  }
  return 0;
}

void GameManaging::SpawnEnemyFormation() {
  if (!enemyManager_) return;

  enemyManager_->SpawnEnemy(EnemyType::Goomba, {400.0f, groundLevel_ - 20.0f});
  enemyManager_->SpawnEnemy(EnemyType::Koopa, {500.0f, groundLevel_ - 30.0f});
  enemyManager_->SpawnEnemy(EnemyType::Piranha, {600.0f, groundLevel_ - 50.0f});
}

void GameManaging::SpawnEnemyWave(int waveNumber) {
  if (!enemyManager_) return;

  for (int i = 0; i < waveNumber; i++) {
    enemyManager_->SpawnEnemy(EnemyType::Goomba, {200.0f + i * 100.0f, groundLevel_ - 20.0f});
  }
}

void GameManaging::CreatePowerUpFromBlock(Vector2 position, int blockType) {
  (void)position;
  (void)blockType; // Power-up creation placeholder
}

void GameManaging::HandleSpecialBlocks(Character *character) {
  (void)character; // Special blocks handling placeholder
}

void GameManaging::SpawnBossEffects() {
  for (int i = 0; i < 10; i++) {
    Vector2 pos = {600.0f + (rand() % 100 - 50), groundLevel_ - 100.0f + (rand() % 50)};
    SpawnParticle(pos, ORANGE, {(float)(rand() % 40 - 20), (float)(rand() % 40 - 20)});
  }
}

void GameManaging::LoadNextLevel() {
  if (levelComplete_) {
    currentLevel_++;
    levelComplete_ = false;
    LoadLevel("");
  }
}

bool GameManaging::IsLevelComplete() const {
  return levelComplete_;
}

bool GameManaging::IsGameOver() const {
  return gameOver_ || lives_ <= 0;
}
