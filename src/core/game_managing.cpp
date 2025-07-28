#include "include/core/game_managing.hpp"
#include "include/enemies/movement_strategy.hpp" // Add this include
#include <algorithm>
#include <raylib.h>
#include <stdexcept>


#include "include/blocks/question_block.hpp"
#include "include/blocks/music_block.hpp"
#include "include/blocks/ground_block.hpp"
#include "include/blocks/empty_block.hpp"
#include "include/blocks/solid_block.hpp"
#include "include/blocks/pipe_block.hpp"
#include "include/blocks/rock_block.hpp"
#include "include/characters/character.hpp"
#include "include/core/application.hpp"
#include "include/core/resource_manager.hpp"
#include "include/enemies/goomba.hpp"
#include "include/enemies/koopa_troopa.hpp"
#include "include/enemies/piranha_plant.hpp"
#include "include/enemies/bowser.hpp"
#include "include/managers/enemy_manager.hpp"

/*GameManaging::GameManaging() {
  lives_            = 3;
  gameTime_         = 0.0f;
  points_           = 0;
  countdownSeconds_ = 300;
  backgroundType_   = 0;
  enemyManager_     = std::make_unique<EnemyManager>();
  updateCounter_    = 0;

  LoadResources();
}*/

/*GameManaging::~GameManaging() {
  UnloadLevel();
}
*/

void GameManaging::LoadLevel(const std::string& path) {
  // Clear previous level data
  UnloadLevel();
  //load map
   
  //get block info

  for (auto& info : blocksInfo) {
      //choose the suitable type
    BlockType b;
      switch (info.type) {
        case 1:
          b = BlockType::Empty;
          break;
        case 2:
          b = BlockType::Solid;
          break;
        case 3:
          b = BlockType::Question;
          break;
        case 4:
          b = BlockType::Music;
          break;
        case 5:
          b = BlockType::Ground;
          break;
        case 6:
          b = BlockType::Rock;
          break;
        case 7:
          b = BlockType::Pipe;
          break;
        default:
          b = BlockType::Empty;
      }

    int blockID = info.gid - 1;

  CreateBlockFromType(
      b, info.gid, info.pos, info.size, blockID, info.solid, info.animating);
  }
  //get background
  background_ = App.Resource().GetBackgroundMap();
  //get enemies
  auto enemies = App.Resource().GetEnemiesMap();
  for (auto& enemy : enemies) {
    EnemyType e;
    switch (enemy.type) {
      case 1:
        e = EnemyType::Goomba;
        break;
      case 2:
        e = EnemyType::Koopa;
        break;
      case 3:
        e = EnemyType::Piranha;
      default:
        e = EnemyType::Bowser;
        break;
    }

    int enemyID = enemy.gid - 1;
    CreateEnemyFromType(e, enemy.pos, enemy.size, enemy.velocity, enemyID);
  } 
  // Create test objects for better performance
  //CreateBlockFromType(1, {100.0f, 600.0f}); // Solid block
  //CreateBlockFromType(2, {200.0f, 600.0f}); // Question block
  //CreateBlockFromType(3, {300.0f, 600.0f}); // Brick block

  // Enhanced enemy spawning with full functionality
  SpawnEnemyFormation();
}

void GameManaging::SpawnEnemyFormation() {
  if (!enemyManager_) return;
  
  // Spawn a variety of enemies to showcase the system
  enemyManager_->SpawnEnemy(EnemyType::Goomba, {400.0f, 600.0f});
  enemyManager_->SpawnEnemy(EnemyType::Koopa, {500.0f, 600.0f});
  enemyManager_->SpawnEnemy(EnemyType::Piranha, {600.0f, 620.0f});
  
  // Spawn a patrol group
  enemyManager_->SpawnPatrolGroup(EnemyType::Goomba, {150.0f, 600.0f}, {350.0f, 600.0f}, 2);
  
  // Spawn jumping enemy
  enemyManager_->SpawnEnemy(EnemyType::Koopa, {700.0f, 600.0f});
  Enemy* jumpingKoopa = nullptr;
  for (Enemy* enemy : enemyManager_->GetEnemies()) {
    if (enemy->GetPosition().x == 700.0f) {
      jumpingKoopa = enemy;
      break;
    }
  }
  if (jumpingKoopa) {
    enemyManager_->MakeEnemyJump(jumpingKoopa, 3.0f, 150.0f);
  }
}

void GameManaging::Update(float deltaTime, Character *activeCharacter) {
  updateCounter_++;
  
  // Update time
  UpdateTime();

  // Update all blocks
  for (auto &block : blocks_) {
    block->Update(dt);
  }

  // ✅ Enhanced enemy management with full functionality
  if (enemyManager_) {
    // Set character references for enemy AI
    if (activeCharacter) {
      Rectangle charRect = activeCharacter->GetRectangle();
      Vector2 charPos = {charRect.x, charRect.y};
      static Vector2 characterPosition;
      characterPosition = charPos;
      enemyManager_->SetCharacterReferences(&characterPosition, &characterPosition, &characterPosition);
    }
    
    // Update all enemies with enhanced AI
    enemyManager_->UpdateAll(deltaTime);
    
    // Handle character interactions
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

  // Update legacy enemies if any
  if (updateCounter_ % 2 == 0) {
    for (auto &enemy : enemies_) {
      if (enemy->IsAlive()) {
        enemy->Update(deltaTime);
      }
    }
  }

  // ✅ Enhanced collision detection with enemy interactions
  if (activeCharacter) {
    CheckSimpleCollisions(activeCharacter);
  }

  // Clean up dead enemies less frequently
  if (updateCounter_ % 20 == 0) {
    RemoveDeadEnemies();
  }
}

void GameManaging::CheckSimpleCollisions(Character* character) {
  if (!character) return;
  
  Rectangle characterRect = character->GetRectangle();
  
  // Simple block collision
  for (auto &block : blocks_) {
    if (block->IsSolid() && block->CheckCollision(characterRect)) {
      Rectangle blockRect = block->GetRect();
      
      // Simple collision response
      if (characterRect.y + characterRect.height > blockRect.y && 
          characterRect.y < blockRect.y + blockRect.height) {
        // Horizontal collision
        character->StopX();
      }
      
      if (characterRect.x + characterRect.width > blockRect.x && 
          characterRect.x < blockRect.x + blockRect.width) {
        // Vertical collision
        if (characterRect.y < blockRect.y) {
          // Hit from above
          character->StopY(blockRect.y - characterRect.height);
        } else {
          // Hit from below
          character->StopY();
          HitBlock(block.get(), character);
        }
      }
    }
  }
  
  // ✅ Note: Enemy collision is now handled by EnemyManager::HandleCharacterInteractions()
  // This provides more sophisticated collision detection and response
}

/*void GameManaging::DrawLevel() {
  DrawBackground();

  // Draw all blocks
  for (const auto &block : blocks_) {
    DrawBlock(block.get());
  }

  // ✅ Enhanced enemy rendering
  if (enemyManager_) {
    DrawEnemiesAdvanced();
  }

  // Draw legacy enemies
  for (const auto &enemy : enemies_) {
    if (enemy->IsAlive()) {
      DrawEnemy(enemy.get());
    }
  }
}

void GameManaging::DrawEnemiesAdvanced() {
  // Draw enemies with enhanced visual effects and state indicators
  for (Enemy* enemy : enemyManager_->GetEnemies()) {
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
          color = ColorBrightness(color, 0.3f); // Brighter when attacking
          break;
        case EnemyState::Shell:
          color = GRAY;
          break;
        default:
          break;
      }
      
      // Apply invulnerability flashing
      if (enemy->IsInvulnerable()) {
        color = ColorAlpha(color, 0.5f);
      }
      
      DrawRectangleRec(rect, color);
      
      // Draw type indicator
      const char* typeName = "";
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
      
      // Draw detection range for debugging (optional)
      #ifdef DEBUG_ENEMY_AI
      if (enemy->IsPlayerInRange(enemy->GetDetectionRange())) {
        DrawCircleLines((int)(rect.x + rect.width/2), (int)(rect.y + rect.height/2), 
                       enemy->GetDetectionRange(), YELLOW);
      }
      #endif
    }
  }
}

void GameManaging::DrawEnemiesSimple() {
  // Fallback simple rendering (kept for compatibility)
  for (Enemy* enemy : enemyManager_->GetEnemies()) {
    if (enemy && enemy->IsAlive()) {
      Rectangle rect = enemy->GetRect();
      Color color = RED;
      
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
      
      DrawRectangleRec(rect, color);
      
      const char* typeName = "";
      switch (enemy->GetType()) {
        case EnemyType::Goomba: typeName = "G"; break;
        case EnemyType::Koopa: typeName = "K"; break;
        case EnemyType::Piranha: typeName = "P"; break;
        case EnemyType::Bowser: typeName = "B"; break;
      }
      DrawText(typeName, (int)rect.x + 2, (int)rect.y + 2, 12, WHITE);
    }
  }
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

/*void GameManaging::DrawBlock(const Block *block) {
  if (!block)
    return;

  // Use fallback rendering for better performance
  Rectangle rect = block->GetRect();
  Color color = GRAY;
  
  switch (block->GetType()) {
    case BlockType::Question:
      color = YELLOW;
      DrawRectangleRec(rect, color);
      DrawText("?", (int)rect.x + 12, (int)rect.y + 8, 16, BLACK);
      break;
    case BlockType::Brick:
      color = BROWN;
      DrawRectangleRec(rect, color);
      DrawText("B", (int)rect.x + 12, (int)rect.y + 8, 16, WHITE);
      break;
    case BlockType::Solid:
      color = GRAY;
      DrawRectangleRec(rect, color);
      DrawText("S", (int)rect.x + 12, (int)rect.y + 8, 16, WHITE);
      break;
    default:
      color = LIGHTGRAY;
      DrawRectangleRec(rect, color);
      break;
  }
  
  DrawRectangleLinesEx(rect, 2, DARKGRAY);
}

/*void GameManaging::DrawEnemy(const Enemy *enemy) {
  if (!enemy)
    return;
  Rectangle rect = enemy->GetRect();
  Color color = RED;
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
      break;
    default:
      color = RED;
      break;
  }
  DrawRectangleRec(rect, color);
}

void GameManaging::DrawStats() const {
  DrawText(TextFormat("Lives: %d", lives_), 10, 40, 24, WHITE);
  DrawText(TextFormat("Points: %d", points_), 10, 70, 24, YELLOW);
  DrawText(
    TextFormat("Time: %d", countdownSeconds_ - (int)gameTime_), 10, 100, 24,
    GREEN);
    
  // ✅ Enhanced enemy stats display
  if (enemyManager_) {
    size_t aliveEnemies = enemyManager_->GetAliveEnemyCount();
    size_t totalEnemies = enemyManager_->GetEnemyCount();
    DrawText(TextFormat("Enemies: %zu/%zu", aliveEnemies, totalEnemies), 10, 130, 16, WHITE);
    
    // Boss status
    if (enemyManager_->HasBoss()) {
      Enemy* boss = enemyManager_->GetBoss();
      if (boss) {
        float healthPercentage = boss->GetHealth() / boss->GetMaxHealth();
        DrawText(TextFormat("Boss HP: %.0f%%", healthPercentage * 100), 10, 150, 20, RED);
        
        // Boss phase indicator (simplified - removed BossMovement reference)
        DrawText("Boss Phase: Active", 10, 170, 16, ORANGE);
      }
    }
    
    // Enemy type breakdown
    auto goombas = enemyManager_->GetEnemiesByType(EnemyType::Goomba);
    auto koopas = enemyManager_->GetEnemiesByType(EnemyType::Koopa);
    auto piranhas = enemyManager_->GetEnemiesByType(EnemyType::Piranha);
    
    int yOffset = 190;
    if (!goombas.empty()) {
      DrawText(TextFormat("Goombas: %zu", goombas.size()), 10, yOffset, 14, BROWN);
      yOffset += 16;
    }
    if (!koopas.empty()) {
      DrawText(TextFormat("Koopas: %zu", koopas.size()), 10, yOffset, 14, GREEN);
      yOffset += 16;
    }
    if (!piranhas.empty()) {
      DrawText(TextFormat("Piranhas: %zu", piranhas.size()), 10, yOffset, 14, DARKGREEN);
      yOffset += 16;
    }
  }
}

void GameManaging::InitializeCollisionSystem(Character* character) {
  // Enhanced collision system initialization
  if (enemyManager_ && character) {
    Rectangle charRect = character->GetRectangle();
    Vector2 charPos = {charRect.x, charRect.y};
    static Vector2 characterPosition;
    characterPosition = charPos;
    enemyManager_->SetCharacterReferences(&characterPosition, &characterPosition, &characterPosition);
  }
}

/*void GameManaging::HitBlock(Block *block, Character *character) {
  if (block) {
    block->OnHit();
    if (block->GetType() == BlockType::Question) {
      AddPoints(50);
    }
  }
}*/

void GameManaging::SpawnEnemy(EnemyType type, Vector2 position) {
  if (enemyManager_) {
    enemyManager_->SpawnEnemy(type, position, 0);
  } else {
    // Fallback to old system
    std::unique_ptr<Enemy> newEnemy;

    switch (type) {
      case EnemyType::Goomba:
        newEnemy = std::make_unique<Goomba>(position, 0);
        break;
      case EnemyType::Koopa:
        newEnemy = std::make_unique<KoopaTroopa>(position, 0);
        break;
      case EnemyType::Piranha:
        newEnemy = std::make_unique<PiranhaPlant>(position, 0);
        break;
      case EnemyType::Bowser:
        newEnemy = std::make_unique<Bowser>(position, 0);
        break;
      default:
        return;
    }

    if (newEnemy) {
      enemies_.push_back(std::move(newEnemy));
    }
  }
}

void GameManaging::SpawnBoss(Vector2 position) {
  backgroundType_ = 2;
  if (enemyManager_) {
    enemyManager_->SpawnBoss(EnemyType::Bowser, position);
  } else {
    SpawnEnemy(EnemyType::Bowser, position);
  }
}

// ✅ Enhanced enemy management methods
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
    std::remove_if(
      enemies_.begin(), enemies_.end(),
      [](const std::unique_ptr<Enemy> &enemy) {
        return !enemy->IsAlive();
      }),
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
    ResetGame();
  }
}

void GameManaging::AddPoints(int points) {
  points_ += points;
}

void GameManaging::ResetGame() {
  lives_ = 3;
  points_ = 0;
  gameTime_ = 0.0f;
  backgroundType_ = 0;
  UnloadLevel();
}

/*void GameManaging::UnloadLevel() {
  blocks_.clear();
  enemies_.clear();
  if (enemyManager_) {
    enemyManager_->ClearAllEnemies();
  }
}

/*void GameManaging::LoadResources() {
  // Resources are automatically loaded by ResManager singleton
}

/*void GameManaging::CreateBlockFromType(int tileType, Vector2 position) {
  std::unique_ptr<Block> newBlock;

  switch (tileType) {
    case 1: // Solid block
      /*newBlock = std::make_unique<StaticBlock>(
        position, 32, 32, BlockType::Solid, tileType);
      break;
    case 2: // Question block
      /*newBlock = std::make_unique<QuestionBlock>(
        position, PowerUpType::Coin, tileType);
      break;
    case 3: // Brick block
      /*newBlock = std::make_unique<StaticBlock>(
        position, 32, 32, BlockType::Brick, tileType);
      break;
    }
     case EnemyType::Bowser: {
      enemies_.push_back(std::make_unique<Goomba>(pos, size, velo, spriteID));
      break;
                           }
    default:
      return;
  }

  if (newBlock) {
    blocks_.push_back(std::move(newBlock));
  }
}*/

/*void GameManaging::CreateEnemyFromType(int enemyType, Vector2 position) {
  SpawnEnemy(static_cast<EnemyType>(enemyType), position);
}

bool GameManaging::IsBossDefeated() const {
  if (enemyManager_) {
    return !enemyManager_->HasBoss();
  }
  return true;
}

int GameManaging::GetBossHP() const {
  if (enemyManager_ && enemyManager_->HasBoss()) {
    Enemy* boss = enemyManager_->GetBoss();
    if (boss) {
      return (int)boss->GetHealth();
    }
  }
  return 0;
}
