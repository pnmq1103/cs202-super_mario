#include "include/items/mushroom_powerup.hpp"

MushroomPowerUp::MushroomPowerUp(Vector2 pos)
  : PowerUp(pos, /*spriteId=*/1) {}

void MushroomPowerUp::ApplyEffect(Player& player) {
  // Player::Grow()
  Deactivate();
}

PowerUpType MushroomPowerUp::GetType() const {
  return PowerUpType::SuperMushroom;
}
