#pragma once
#include "include/items/powerup.hpp"

class FireFlowerPowerUp : public PowerUp {
public:
  FireFlowerPowerUp(Vector2 pos);
  void ApplyEffect(Player& player) override;
  PowerUpType GetType() const override;
};
