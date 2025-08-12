#pragma once
#include <unordered_map>

#include "include/external/json.hpp"

class GameInfo {
private:
  std::unordered_map<int, int> level_start_points;
  std::unordered_map<int, int> level_complete_times;

  friend struct nlohmann::adl_serializer<GameInfo>;

private:
  GameInfo() : life(3), coin(0), game_time(300) {}
  GameInfo(const GameInfo &info)            = delete;
  GameInfo &operator=(const GameInfo &info) = delete;

public:
  int life;
  int coin;
  int game_time;

public:
  static GameInfo &GetInstance();
  void Reset();
  void ResetLives();
  void SetInitialPointsForLevel(int level, int points);
  int GetInitialPointsForLevel(int level);
  bool HasInitialPointsForLevel(int level) const;
  void SetLevelCompletionTime(int level, int seconds);
  int GetLevelCompletionTime(int level) const;
  bool HasCompletedLevel(int level) const;
};

namespace nlohmann {
  template <>
  struct adl_serializer<GameInfo> {
    static void to_json(json &j, const GameInfo &info) {
      j = json{
        {"life", info.life},
        {"coin", info.coin},
        {"game_time", info.game_time},
        {"level_start_points", info.level_start_points},
        {"level_complete_times", info.level_complete_times}};
    }

    static void from_json(const json &j, GameInfo &info) {
      j.at("life").get_to(info.life);
      j.at("coin").get_to(info.coin);
      j.at("game_time").get_to(info.game_time);
      j.at("level_start_points").get_to(info.level_start_points);
      j.at("level_complete_times").get_to(info.level_complete_times);
    }
  };
} // namespace nlohmann
