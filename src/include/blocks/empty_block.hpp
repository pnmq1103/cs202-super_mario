#pragma once
#include "include/blocks/block.hpp"

class EmptyBlock : public Block {
private:
public:
  EmptyBlock(
    int gid, Vector2 pos, Vector2 size, int spriteID, bool solid,
    bool animating);

  void OnHit() override;
  void Update(float dt) override;

  Block *Clone() const override;
};
