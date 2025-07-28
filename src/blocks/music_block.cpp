#include "include/blocks/music_block.hpp"
#include "include/items/spawn_item.hpp"

MusicBlock::MusicBlock(
  int gid, Vector2 pos, Vector2 size, int spriteId, bool solid, bool animating)
    : Block(gid, pos, size, BlockType::Music, spriteId, solid, animating) {
  chosen = GetLoot();
}

PowerUpType MusicBlock::GetLoot() {
  std::unordered_map<PowerUpType, float> loots = {
    {PowerUpType::FireFlower, 0.3},
    {PowerUpType::Star, 0.3},
    {PowerUpType::SuperMushroom, 0.3}};
  // calculate drop item
  float sum = 0.0;
  for (auto &loot1 : loots)
    sum += loot1.second;
  float range = GetRandomValue(0, 9) / 10.0f;
  if (range >= sum)
    return PowerUpType::None;
  else {
    float val = 0;
    for (auto &loot2 : loots) {
      val += loot2.second;
      if (range < val)
        return loot2.first;
    }
  }
  return PowerUpType::None;
}

void MusicBlock::OnHit() {
  if (used)
    return;
  // if not coin
  if (chosen != PowerUpType::None && chosen != PowerUpType::Coin) {
    SpawnItem(chosen, position);
    used = true;
  }
}

void MusicBlock::Update(float dt) {}

bool MusicBlock::IsUsed() const {
  return used;
}

PowerUpType MusicBlock::GetPowerUp() const {
  return chosen;
}

Block *MusicBlock::Clone() const {
  return new MusicBlock(*this);
}
