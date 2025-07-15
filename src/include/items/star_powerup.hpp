#pragma once
#include "include/items/powerup.hpp"

class StarPowerUp : public PowerUp {
public:
  StarPowerUp(Vector2 pos);
  void ApplyEffect(Player& player) override;
  PowerUpType GetType() const override;
};
