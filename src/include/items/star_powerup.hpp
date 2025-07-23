#pragma once
#include "include/items/powerup.hpp"

class StarPowerUp : public PowerUp {
public:
  StarPowerUp(Vector2 pos);
  void ApplyEffect() override;
  PowerUpType GetType() const override;
};
