#include "include/blocks/question_block.hpp"
#include "include/items/spawn_item.hpp"

QuestionBlock::QuestionBlock(int gid, Vector2 pos, Vector2 size, int spriteId, bool solid, bool animating)
    : Block(gid, pos, size, BlockType::Question, spriteId, solid, animating) {
  chosen = GetLoot();
}

PowerUpType QuestionBlock::GetLoot() {
  std::unordered_map<PowerUpType, float> loots = {
    {PowerUpType::Coin, 0.4},
    {PowerUpType::FireFlower, 0.2},
    {PowerUpType::Star, 0.2},
    {PowerUpType::SuperMushroom, 0.2}};
  // calculate drop item
  float sum = 0.0;
  for (auto &loot1 : loots)
    sum += loot1.second;
  float range = GetRandomValue(0,9) / 10.0f;
  if (range >= sum)
    return PowerUpType::None;
  else {
    float val = 0;
    for (auto& loot2 : loots) {
      val += loot2.second;
      if (range < val)
        return loot2.first;
    }
  }
  return PowerUpType::None;
}

void QuestionBlock::OnHit() {
  if (isUsed)
    return;
  isHit = true;
  timer = 0.0f;
  //if not coin
  if (chosen != PowerUpType::None && chosen != PowerUpType::Coin) {
    if (!isUniqueLoot) {
      SpawnItem(chosen, position);
      isUniqueLoot = true;
    } else isUsed = true;
  } else { //if coin
    if (coinCount < maxCoins) {
      SpawnItem(chosen, position);
      coinCount++;
    } else isUsed = true;
  }
}

void QuestionBlock::Update(float dt) {
    if (isHit) {
    timer += dt;
    float t = timer / 0.2f; //animation time is 0.2 second
    float offset = 0.4f * ((t > 0.5f) ? (2 * t) : (2 * (1 - t))); //calculate height of animation
    Vector2 startPos = position;
    Vector2 endPos   = {startPos.x, startPos.y - offset};
      SetPosition(endPos);
    if (timer >= 0.2f) {
      isHit = false;
      SetPosition(endPos);
      }

  }
}

bool QuestionBlock::IsUsed() const {
  return isUsed;
}

PowerUpType QuestionBlock::GetPowerUp() const {
  return chosen;
}

Block *QuestionBlock::Clone() const {
  return new QuestionBlock(*this);
}
