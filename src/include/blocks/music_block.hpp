#pragma once
#include "include/blocks/block.hpp"

class MusicBlock : public Block {
private:
  PowerUpType chosen;
  bool used         = false;

  PowerUpType GetLoot();

public:
  MusicBlock(
    int gid, Vector2 pos, Vector2 size, int spriteID, bool solid,
    bool animating);

  void OnHit() override;
  void Update(float dt) override;

  bool IsUsed() const;
  PowerUpType GetPowerUp() const;

  Block *Clone() const override;
};
