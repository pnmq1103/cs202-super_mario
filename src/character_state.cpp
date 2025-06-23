#include "include/character_state.hpp"

CharacterState::CharacterState(float Nspeed, int NjT, float NjH, float Nscale)
    : speed(Nspeed), jT(NjT), jH(NjH), scale(Nscale) {
  tY = -1;
  tX = -1;
  t = 0;
}

CharacterState::~CharacterState() { UnloadTexture(texture); }

Vector2 CharacterState::getPosition() { return pos; }

Vector2 CharacterState::getSize() {
  return {static_cast<float>(texture.width),
          static_cast<float>(texture.height)};
}

void CharacterState::setPosition(Vector2 Npos) { pos = Npos; }

void CharacterState::setFrameCount() { ++t; }

void CharacterState::draw() { DrawTextureRec(texture, frame, pos, WHITE); }

void CharacterState::jump() {
  if (tY == -1) {
    tY = t;
    y = pos.y;
  }
}

void CharacterState::run(bool left) {
  if (tX == -1) {
    tX = t;
    this->left = left;
  }
}