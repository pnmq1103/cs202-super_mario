#pragma once
#include <iostream>
#include <fstream>
#include <map>
#include <string>

class GameInfo {
private:
  GameInfo() : life(3), coin(0), game_times(300) {}
  GameInfo(const GameInfo &info)            = delete;
  GameInfo &operator=(const GameInfo &info) = delete;
  std::map<int, int> levelStartPoints;
  std::map<int, int> levelCompletionTimes;

public:
  static GameInfo &GetInstance() {
    static GameInfo instance;
    return instance;
  }

  int life;
  int coin;
  int game_times;
  void ResetLives() {
    life = 3;
  }
  void Reset() {
    life  = 3;
    coin  = 0;
    game_times  = 300;
    levelStartPoints.clear();
    levelCompletionTimes.clear();
  }
  void SaveToFile(const std::string &filename = "game_save.txt") const {
    try {
      std::ofstream outFile(filename);
      if (!outFile.is_open()) {
        std::cerr << "Failed to open file for saving game info: " << filename
                  << std::endl;
        return;
      }

      outFile << life << std::endl;
      outFile << coin << std::endl;
      outFile << game_times << std::endl;

      outFile << levelCompletionTimes.size() << std::endl;

      // Save each level completion time (level number and time)
      for (const auto &[level, time] : levelCompletionTimes) {
        outFile << level << " " << time << std::endl;
      }

      outFile.close();
      std::cout << "Game info saved to: " << filename << std::endl;
    } catch (const std::exception &e) {
      std::cerr << "Error saving game info: " << e.what() << std::endl;
    }
  }
  void SetInitialPointsForLevel(int level, int points) {
    // Only set if not already set
    if (levelStartPoints.find(level) == levelStartPoints.end()) {
      levelStartPoints[level] = points;
      std::cout << "Setting initial points for level " << level << ": "
                << points << std::endl;
    }
  }
  int GetInitialPointsForLevel(int level) {
    // Level 1 always starts with 0 points
    if (level == 1)
      return 0;

    // For other levels, return stored value or 0 if not found
    if (levelStartPoints.find(level) != levelStartPoints.end()) {
      return levelStartPoints[level];
    }
    return 0;
  }
  bool HasInitialPointsForLevel(int level) const {
    return levelStartPoints.find(level) != levelStartPoints.end();
  }
  // Set the completion time for a level
  void SetLevelCompletionTime(int level, int seconds) {
    levelCompletionTimes[level] = seconds;
    std::cout << "Level " << level << " completed in " << seconds << " seconds!"
              << std::endl;
  }

  // Get the completion time for a level
  int GetLevelCompletionTime(int level) const {
    auto it = levelCompletionTimes.find(level);
    if (it != levelCompletionTimes.end()) {
      return it->second;
    }
    return 0; // Return 0 if level hasn't been completed
  }

  // Check if a level has been completed
  bool HasCompletedLevel(int level) const {
    return levelCompletionTimes.find(level) != levelCompletionTimes.end();
  }
 };