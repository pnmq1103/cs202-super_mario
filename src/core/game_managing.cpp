#include <algorithm>
#include <raylib.h>
#include <stdexcept>

#include "include/blocks/question_block.hpp"
#include "include/blocks/static_block.hpp"
#include "include/characters/character.hpp"
#include "include/core/application.hpp"
#include "include/core/game_managing.hpp"
#include "include/core/resource_manager.hpp"
#include "include/enemies/goomba.hpp"
#include "include/enemies/koopa_troopa.hpp"

GameManaging::GameManaging() {
  lives_            = 3;
  gameTime_         = 0.0f;
  points_           = 0;
  countdownSeconds_ = 300;
  backgroundType_   = 0;

  LoadResources();
}

GameManaging::~GameManaging() {
  UnloadLevel();
  // No need to manually unload textures - ResManager handles this
}

void GameManaging::LoadLevel(const std::string &filename) {
  // Clear previous level data
  UnloadLevel();

  (void)filename; // Suppress unused parameter warning

  // For now, create some example blocks and enemies
  // TODO: Use FileHandler to load binary level data when file format is ready

  // Example blocks
  CreateBlockFromType(1, {100.0f, 600.0f}); // Solid block
  CreateBlockFromType(2, {200.0f, 600.0f}); // Question block
  CreateBlockFromType(3, {300.0f, 600.0f}); // Brick block

  // Example enemies
  SpawnEnemy(EnemyType::Goomba, {200.0f, 500.0f});
  SpawnEnemy(EnemyType::Koopa, {400.0f, 500.0f});
}

void GameManaging::Update(float deltaTime, Character *activeCharacter) {
  // Update time
  UpdateTime();

  // Update all blocks
  for (auto &block : blocks_) {
    block->Update(deltaTime);
  }

  // Update all enemies
  for (auto &enemy : enemies_) {
    if (enemy->IsAlive()) {
      enemy->Update(deltaTime);
    }
  }

  // Check collisions if character exists
  if (activeCharacter) {
    CheckCollisions(activeCharacter);
  }

  // Remove dead enemies
  RemoveDeadEnemies();
}

void GameManaging::DrawLevel() {
  DrawBackground();

  // Draw all blocks using ResManager
  for (const auto &block : blocks_) {
    DrawBlock(block.get());
  }

  // Draw all enemies using ResManager
  for (const auto &enemy : enemies_) {
    if (enemy->IsAlive()) {
      DrawEnemy(enemy.get());
    }
  }
}

void GameManaging::DrawBackground() {
  // Draw background using ResManager
  try {
    Texture backgroundTex = App.Resource().GetBackground();
    DrawTexture(backgroundTex, 0, 0, WHITE);
  } catch (const std::out_of_range &) {
    // Fallback to color backgrounds if texture not found
    switch (backgroundType_) {
      case 0: // Overworld
        ClearBackground(SKYBLUE);
        break;
      case 1: // Underground
        ClearBackground(BLACK);
        break;
      case 2: // Castle
        ClearBackground(DARKGRAY);
        break;
      default:
        ClearBackground(SKYBLUE);
        break;
    }
  }
}

void GameManaging::DrawBlock(const Block *block) {
  if (!block)
    return;

  try {
    // Get tile texture from ResManager based on sprite ID
    Texture tileTexture = App.Resource().GetTile(block->GetSpriteId() + 1);
    Vector2 position    = block->GetPosition();
    DrawTexture(tileTexture, (int)position.x, (int)position.y, WHITE);
  } catch (const std::out_of_range &) {
    // Fallback to simple rectangle if texture not found
    Rectangle rect = block->GetRect();
    Color color    = GRAY;
    switch (block->GetType()) {
      case BlockType::Question:
        color = YELLOW;
        break;
      case BlockType::Brick:
        color = BROWN;
        break;
      case BlockType::Solid:
        color = GRAY;
        break;
      default:
        color = LIGHTGRAY;
        break;
    }
    DrawRectangleRec(rect, color);
  }
}

void GameManaging::DrawEnemy(const Enemy *enemy) {
  if (!enemy)
    return;

  // For now, use fallback rectangles since Enemy doesn't have GetSpriteId yet
  Rectangle rect = enemy->GetRect();
  Color color    = RED;
  switch (enemy->GetType()) {
    case EnemyType::Goomba:
      color = BROWN;
      break;
    case EnemyType::Koopa:
      color = GREEN;
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
}

void GameManaging::CheckCollisions(Character *character) {
  if (!character)
    return;

  Rectangle characterRect = character->GetRectangle();

  // Check block collisions
  Vector2 correction = {0, 0};
  if (CheckBlockCollision(characterRect, correction)) {
    // Apply collision correction to character
    // Character doesn't have SetPosition, so we'll use StopY for vertical
    // corrections
    if (correction.y != 0) {
      if (correction.y < 0) {
        // Character hit something from below
        character->StopY();
      } else {
        // Character landed on something
        character->StopY(characterRect.y + correction.y);
      }
    }
    if (correction.x != 0) {
      // For horizontal collisions, stop horizontal movement
      character->StopX();
    }
  }

  // Check enemy collisions
  CheckEnemyCollisions(character);
}

bool GameManaging::CheckBlockCollision(
  Rectangle characterRect, Vector2 &correction) {
  bool collided = false;
  correction    = {0, 0};

  for (auto &block : blocks_) {
    if (block->IsSolid() && block->CheckCollision(characterRect)) {
      Rectangle blockRect = block->GetRect();

      // Calculate collision correction
      float overlapX = std::min(
        characterRect.x + characterRect.width - blockRect.x,
        blockRect.x + blockRect.width - characterRect.x);
      float overlapY = std::min(
        characterRect.y + characterRect.height - blockRect.y,
        blockRect.y + blockRect.height - characterRect.y);

      if (overlapX < overlapY) {
        // Horizontal collision
        if (characterRect.x < blockRect.x) {
          correction.x = -overlapX;
        } else {
          correction.x = overlapX;
        }
      } else {
        // Vertical collision
        if (characterRect.y < blockRect.y) {
          correction.y = -overlapY;
          // Character hit block from below
          HitBlock(block.get(), nullptr);
        } else {
          correction.y = overlapY;
          // Character landed on block
        }
      }
      collided = true;
      break; // Handle one collision at a time
    }
  }

  return collided;
}

void GameManaging::CheckEnemyCollisions(Character *character) {
  Rectangle characterRect = character->GetRectangle();

  for (auto &enemy : enemies_) {
    if (enemy->IsAlive() && enemy->CheckCollision(characterRect)) {
      // Determine collision type
      Vector2 enemyPos = enemy->GetPosition();
      Vector2 charPos  = {characterRect.x, characterRect.y};

      if (charPos.y + characterRect.height <= enemyPos.y + 10) {
        // Character hit enemy from above
        enemy->OnHitFromAbove();
        AddPoints(100); // Award points for defeating enemy
      } else {
        // Enemy hit character from side
        enemy->OnHitFromSide();
        character->Die(); // Character takes damage
        DecreaseLife();
      }
    }
  }
}

void GameManaging::HitBlock(Block *block, Character *character) {
  if (block) {
    block->OnHit();
    // Award points for hitting certain blocks
    if (block->GetType() == BlockType::Question) {
      AddPoints(50);
    }
  }
}

void GameManaging::SpawnEnemy(EnemyType type, Vector2 position) {
  std::unique_ptr<Enemy> newEnemy;

  switch (type) {
    case EnemyType::Goomba:
      newEnemy = std::make_unique<Goomba>(position, 0);
      break;
    case EnemyType::Koopa:
      newEnemy = std::make_unique<KoopaTroopa>(position, 0);
      break;
    default:
      return;
  }

  if (newEnemy) {
    enemies_.push_back(std::move(newEnemy));
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
  gameTime_  += delta;
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
  lives_    = 3;
  points_   = 0;
  gameTime_ = 0.0f;
  UnloadLevel();
}

void GameManaging::UnloadLevel() {
  blocks_.clear();
  enemies_.clear();
}

void GameManaging::LoadResources() {
  // Resources are automatically loaded by ResManager singleton
  // No need to manually load textures here
}

void GameManaging::CreateBlockFromType(int tileType, Vector2 position) {
  std::unique_ptr<Block> newBlock;

  // Map tile types to block types
  switch (tileType) {
    case 1: // Solid block
      newBlock = std::make_unique<StaticBlock>(
        position, 32, 32, BlockType::Solid, tileType);
      break;
    case 2: // Question block
      newBlock = std::make_unique<QuestionBlock>(
        position, PowerUpType::Coin, tileType);
      break;
    case 3: // Brick block
      newBlock = std::make_unique<StaticBlock>(
        position, 32, 32, BlockType::Brick, tileType);
      break;
    default:
      return; // Unknown tile type
  }

  if (newBlock) {
    blocks_.push_back(std::move(newBlock));
  }
}

void GameManaging::CreateEnemyFromType(int enemyType, Vector2 position) {
  SpawnEnemy(static_cast<EnemyType>(enemyType), position);
}
