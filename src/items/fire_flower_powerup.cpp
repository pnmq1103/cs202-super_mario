#include "include/items/fire_flower_powerup.hpp"

FireFlowerPowerUp::FireFlowerPowerUp(Vector2 pos)
  : PowerUp(pos, /*spriteId=*/2) {} 

void FireFlowerPowerUp::ApplyEffect(Player& player) {
  // player.EnableFireMode(); 
  Deactivate();
}

PowerUpType FireFlowerPowerUp::GetType() const {
  return PowerUpType::FireFlower;
}
