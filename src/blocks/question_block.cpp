#include "../include/blocks/question_block.hpp"

QuestionBlock::QuestionBlock(Vector2 pos, PowerUpType item, int spriteId)
    : Block(pos, 16, 16, BlockType::Question, true, spriteId), item(item) {}

void QuestionBlock::OnHit() {
  if (used)
    return;

  if (item == PowerUpType::Coin) {
    coinCount++;
    if (coinCount >= maxCoins) {
      used     = true;
      type     = BlockType::Solid;
      spriteId = 999;
    }
  } else {
    used     = true;
    type     = BlockType::Solid;
    spriteId = 999;
  }
}

void QuestionBlock::Update(float dt) {}

bool QuestionBlock::IsUsed() const {
  return used;
}

PowerUpType QuestionBlock::GetItem() const {
  return item;
}

Block *QuestionBlock::Clone() const {
  return new QuestionBlock(*this);
}
