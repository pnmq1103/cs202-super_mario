#pragma once
class GameInfo {
private:
  GameInfo()                                = default;
  GameInfo(const GameInfo &info)            = delete;
  GameInfo &operator=(const GameInfo &info) = delete;

public:
  int life;
  int coin;
  GameInfo &GetInstant();
};