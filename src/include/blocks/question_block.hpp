#pragma once
#include "block.hpp"

enum class PowerUpType {
  None,
  Coin,
  SuperMushroom,
  FireFlower,
  Star,
};

class QuestionBlock : public Block {
private:
  int coinCount = 0;
  int maxCoins  = 10;
  bool used     = false;
  PowerUpType item;

public:
  QuestionBlock(
    Vector2 pos, PowerUpType item = PowerUpType::Coin, int spriteId = 0);

  void onHit() override;
  void update(float dt) override;

  bool isUsed() const;
  PowerUpType getItem() const;

  Block *clone() const override;
};
