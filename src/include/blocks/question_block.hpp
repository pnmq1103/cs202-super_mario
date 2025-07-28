#pragma once
#include "include/blocks/block.hpp"

class QuestionBlock : public Block {
private:
  int coinCount = 0;
  int maxCoins  = 10;
  PowerUpType chosen;
  bool isUniqueLoot = false;
  bool isUsed     = false;
  bool isHit        = false;
  float timer       = 0.0f;

  PowerUpType GetLoot();
  

public:
  QuestionBlock(int gid, Vector2 pos, Vector2 size, int spriteID, bool solid, bool animating);

  void OnHit() override;
  void Update(float dt) override;

  bool IsUsed() const;
  PowerUpType GetPowerUp() const;

  Block *Clone() const override;
};
