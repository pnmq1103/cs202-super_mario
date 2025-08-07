#pragma once
#include "include/enemies/enemy.hpp"

class PiranhaPlant : public Enemy {
private:
  bool is_emerging;      // Whether plant is currently emerging (vs hiding)
  float emerge_timer;    // Timer for emergence/hiding cycle
  float emerge_duration; // How long to stay emerged (seconds)
  float hide_duration;   // How long to stay hidden (seconds)
  float emerge_distance; // How far to emerge from pipe (pixels)
  float initial_y;       // Initial Y position (fully hidden)
  float emerge_speed;    // Speed of movement during emergence/hiding

public:
  PiranhaPlant(Vector2 pos, float Nscale);
  ~PiranhaPlant();

  void OnHit() override;
  void Update() override;
  void OnHitFromAbove() override;
  void OnHitFromSide() override;
  EnemyType GetType() override;
  Enemy *Clone() const override;

  // Emergence behavior methods
  void StartEmerging();
  void StartHiding();
  bool IsEmerging() const;
  void ForceEmerge();
  void ForceHide();
};