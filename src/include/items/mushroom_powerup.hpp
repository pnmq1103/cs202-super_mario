#pragma once
#include "include/items/powerup.hpp"

class MushroomPowerUp : public PowerUp {
public:
  MushroomPowerUp(Vector2 pos);
  void ApplyEffect(Player& player) override;
  PowerUpType GetType() const override;
};
