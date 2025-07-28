#pragma once
#include "include/blocks/block.hpp"

class SolidBlock : public Block {
private:
public:
  SolidBlock(
    int gid, Vector2 pos, Vector2 size, int spriteID, bool solid,
    bool animating);

  void OnHit() override;
  void Update(float dt) override;

  Block *Clone() const override;
};
