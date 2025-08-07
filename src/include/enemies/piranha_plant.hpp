#pragma once
#include "include/enemies/enemy.hpp"

class PiranhaPlant : public Enemy {
private:
  bool is_emerging; // New member names to match implementation
  float emerge_timer;

public:
  PiranhaPlant(Vector2 pos, float Nscale);
  ~PiranhaPlant();

  void OnHit() override;
  void Update() override;
  void OnHitFromAbove() override;
  void OnHitFromSide() override;
  EnemyType GetType() override;
  Enemy *Clone() const override;

  // New methods for emergence behavior
  void StartEmerging();
  void StartHiding();
  bool IsEmerging() const;
  void ForceEmerge();
  void ForceHide();
};
