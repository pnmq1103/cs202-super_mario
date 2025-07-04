#include "../include/blocks/question_block.hpp"

QuestionBlock::QuestionBlock(Vector2 pos, PowerUpType item, int spriteId)
    : Block(pos, 16, 16, BlockType::Question, true, spriteId), item(item) {}

void QuestionBlock::onHit() {
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

void QuestionBlock::update(float dt) {}

bool QuestionBlock::isUsed() const {
  return used;
}

PowerUpType QuestionBlock::getItem() const {
  return item;
}

Block *QuestionBlock::clone() const {
  return new QuestionBlock(*this);
}
