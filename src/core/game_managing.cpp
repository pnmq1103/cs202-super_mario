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
    Texture tileTexture = App.Resource().GetTileset(block->GetSpriteId() + 1);
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
      case BlockType::Music:
        color = GRAY;
        break;
      case BlockType::Solid:
        color = BROWN;
        break;
      case BlockType::Rock:
        color = DARKGRAY;
        break;
      case BlockType::Empty:
        color = WHITE;
        break;
      case BlockType::Ground:
        color = DARKBROWN;
        break;
      case BlockType::Pipe:
          color = DARKGREEN;
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
  Texture tileTexture = App.Resource().GetEnemy(enemy->GetSpriteId() + 1);
  Vector2 position    = enemy->GetPosition();
  DrawTexture(tileTexture, (int)position.x, (int)position.y, WHITE);
  Rectangle rect = enemy->GetRect();
  Color color    = RED;
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

//dont need this function right now
//void GameManaging::SpawnEnemy(EnemyType type, Vector2 position) {
//  std::unique_ptr<Enemy> newEnemy;
//
//  switch (type) {
//    case EnemyType::Goomba:
//      newEnemy = std::make_unique<Goomba>(position, 0);
//      break;
//    case EnemyType::Koopa:
//      newEnemy = std::make_unique<KoopaTroopa>(position, 0);
//      break;
//    default:
//      return;
//  }
//
//  if (newEnemy) {
//    enemies_.push_back(std::move(newEnemy));
//  }
//}

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

void GameManaging::CreateBlockFromType(
  BlockType type, int gid, Vector2 pos, Vector2 size, int spriteID, bool solid,
  bool animating) {
  switch (type) {
    case BlockType::Empty: {
      blocks_.push_back(std::make_unique<EmptyBlock>(
        gid, pos, size, spriteID, solid, animating));
      break;
    }
    case BlockType::Solid: {
      blocks_.push_back(std::make_unique<SolidBlock>(
        gid, pos, size, spriteID, solid, animating));
      break;
    }
    case BlockType::Question: {
      blocks_.push_back(std::make_unique<QuestionBlock>(
        gid, pos, size, spriteID, solid, animating));
      break;
    }
    case BlockType::Music: {
      blocks_.push_back(std::make_unique<MusicBlock>(
        gid, pos, size, spriteID, solid, animating));
      break;
    }
    case BlockType::Ground: {
      blocks_.push_back(std::make_unique<GroundBlock>(
        gid, pos, size, spriteID, solid, animating));
      break;
    }
    case BlockType::Rock: {
      blocks_.push_back(std::make_unique<RockBlock>(
        gid, pos, size, spriteID, solid, animating));
      break;
    }
    case BlockType::Pipe: {
      blocks_.push_back(std::make_unique<PipeBlock>(
        gid, pos, size, spriteID, solid, animating));
      break;
    }
    default: {
      throw std::invalid_argument("Unknown block type");
      break;
  }
  }
}

void GameManaging::CreateEnemyFromType(EnemyType type, Vector2 pos, Vector2 size, Vector2 velo, int spriteID) {
  switch (type) {
    case EnemyType::Goomba: {
      enemies_.push_back(std::make_unique<Goomba>(pos, size, velo, spriteID));
      break;
    }
    case EnemyType::Koopa: {
      enemies_.push_back(std::make_unique<KoopaTroopa>(pos, size, velo, spriteID));
      break;
    }
    case EnemyType::Piranha: {
      enemies_.push_back(std::make_unique<Goomba>(pos,size, velo, spriteID));
      break;
    }
     case EnemyType::Bowser: {
      enemies_.push_back(std::make_unique<Goomba>(pos, size, velo, spriteID));
      break;
                           }
    default:
      throw std::invalid_argument("Unknown enemy type");
  }
}
