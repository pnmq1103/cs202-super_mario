#include "include/core/game_info.hpp"
GameInfo &GameInfo::GetInstant() {
  static GameInfo info;
  return info;
}