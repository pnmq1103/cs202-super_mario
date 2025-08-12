#include <iostream>

#include "include/core/game_info.hpp"

GameInfo &GameInfo::GetInstance() {
  static GameInfo instance;
  return instance;
}

void GameInfo::Reset() {
  life      = 3;
  coin      = 0;
  game_time = 300;
  level_start_points.clear();
  level_complete_times.clear();
}

void GameInfo::ResetLives() {
  life = 3;
}

void GameInfo::SetInitialPointsForLevel(int level, int points) {
  // Only set if not already set
  if (level_start_points.find(level) == level_start_points.end()) {
    level_start_points[level] = points;
    std::cout << "Setting initial points for level " << level << ": " << points
              << std::endl;
  }
}

int GameInfo::GetInitialPointsForLevel(int level) {
  // Level 1 always starts with 0 points
  if (level == 1)
    return 0;

  // For other levels, return stored value or 0 if not found
  if (level_start_points.find(level) != level_start_points.end()) {
    return level_start_points[level];
  }
  return 0;
}

bool GameInfo::HasInitialPointsForLevel(int level) const {
  return level_start_points.find(level) != level_start_points.end();
}

void GameInfo::SetLevelCompletionTime(int level, int seconds) {
  level_complete_times[level] = seconds;
  std::cout << "Level " << level << " completed in " << seconds << " seconds!"
            << std::endl;
}

int GameInfo::GetLevelCompletionTime(int level) const {
  auto it = level_complete_times.find(level);
  if (it != level_complete_times.end()) {
    return it->second;
  }
  return 0; // If level hasn't been completed
}

bool GameInfo::HasCompletedLevel(int level) const {
  return level_complete_times.find(level) != level_complete_times.end();
}
