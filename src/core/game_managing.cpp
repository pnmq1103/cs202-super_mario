#include "include/game_managing.hpp"
#include "include/file_handler.hpp"
#include <cstring>
#include <raylib.h>

GameManaging::GameManaging() {
  lives_            = 3;
  gameTime_         = 0.0f;
  points_           = 0;
  countdownSeconds_ = 300; // Example: 300 seconds per level
  backgroundType_   = 0;
}

void GameManaging::LoadLevel(const std::string &filename) {
  // Use FileHandler to load binary level data
  FileHandler fh;
  SaveDatawMap sd;
  if (fh.loadFile(filename, sd)) {
    // Set background type
    backgroundType_ = sd.header.backgroundID;
    // Clear previous blocks
    blocks_.clear();
    // Convert tileData to Block
    for (const auto &tile : sd.mapTiles) {
      Block b;
      b.position = {static_cast<float>(tile.x), static_cast<float>(tile.y)};
      b.type     = tile.tileType;
      blocks_.push_back(b);
    }
    // TODO: Load enemies if present in file (extend FileHandler if needed)
  }
}

void GameManaging::DrawLevel() {
  // TODO: Draw background based on backgroundType_
  // TODO: Draw blocks
  for (const auto &block : blocks_) {
    // Draw block sprite at block.position
    // Use Raylib's DrawTexture or DrawRectangle as placeholder
    DrawRectangle(block.position.x, block.position.y, 32, 32, GRAY);
  }
  // TODO: Draw enemies
  for (const auto &enemy : enemies_) {
    // Draw enemy sprite at enemy.position
    DrawRectangle(enemy.position.x, enemy.position.y, 32, 32, RED);
  }
}

void GameManaging::DrawStats() {
  // Display lives, points, and time using Raylib's DrawText
  DrawText(TextFormat("Lives: %d", lives_), 10, 10, 24, RED);
  DrawText(TextFormat("Points: %d", points_), 10, 40, 24, RED);
  DrawText(
    TextFormat("Time: %d", countdownSeconds_ - (int)gameTime_), 10, 70, 24,
    RED);
}

void GameManaging::UpdateTime() {
  float delta = GetFrameTime();
  gameTime_  += delta;
  if (countdownSeconds_ - (int)gameTime_ <= 0) {
    // Time's up, handle as needed (e.g., lose a life)
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

void GameManaging::ResetGame() {
  lives_    = 3;
  points_   = 0;
  gameTime_ = 0.0f;
  // TODO: Reload level or return to menu
}
