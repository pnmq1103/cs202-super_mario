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

  void OnHit() override;
  void Update(float dt) override;

  bool IsUsed() const;
  PowerUpType GetItem() const;

  Block *Clone() const override;
};
