#pragma once
#include "include/core/application.hpp"
#include <cmath>
#include <fstream>
#include <iostream>
#include <raylib.h>
#include <unordered_map>
#include <vector>

// Forward declaration
class MovementStrategy;

enum class EnemyType {
  Goomba,
  Koopa,
  Piranha,
  Bowser,
};

enum class EnemyState { Normal, Stunned, Shell, Dead, Attacking, Hiding };

class Enemy {
private:
  // Static members similar to GameObject pattern
  static int enemy_count_;
  int index_;

protected:
  // Static frame counter for animations, similar to GameObject
  static int time;

  Vector2 position;
  const float scale, gravity;
  bool alive;

  // Enemy type and state
  EnemyType type;
  EnemyState state;

  // Sprite handling similar to GameObject
  const Texture *texture;
  Rectangle frame;
  std::vector<Rectangle> frame_list;

  // Physics similar to GameObject
  Vector2 velocity;
  float y_mark;

  // Enemy-specific properties
  float health;
  float max_health;
  float stun_timer;
  float invulnerability_timer;
  bool facing_left;
  float detection_range;

  // Movement strategy
  MovementStrategy *movement_strategy_;

  // Player reference for AI
  Vector2 *player_position;

  // Helper methods similar to GameObject
  Rectangle MakeDestRect(Rectangle rect);
  void LoadFrameList(std::string address);
  void Bounce();
  void UpdateAnimationFrame();

public:
  // Static methods for frame management like GameObject
  static void SetFrameCount();
  static void Reset();

  Enemy(Vector2 Nposition, float Nscale, EnemyType enemy_type);
  virtual ~Enemy();

  // Core functionality similar to GameObject
  virtual Rectangle GetRectangle() const;
  bool IsAlive() const;
  virtual void OnHit() = 0;
  virtual void Update();
  virtual void Draw();
  virtual EnemyType GetType() = 0;

  // Enemy-specific virtual methods
  virtual void OnHitFromAbove() = 0;
  virtual void OnHitFromSide()  = 0;
  virtual Enemy *Clone() const  = 0;

  // Movement and AI
  void SetMovementStrategy(MovementStrategy *strategy);
  MovementStrategy *GetMovementStrategy() const;
  void UpdateMovement(float dt);
  void ReverseDirection();    // Made public for collision handler
  void HandleWallCollision(); // Added missing method

  // Physics similar to GameObject
  virtual void StopY(float Ny);
  void HandleGroundCollision(float ground_level = 600.0f);

  // Getters and setters
  Vector2 GetPosition() const;
  Vector2 GetVelocity() const;
  EnemyState GetState() const;
  float GetHealth() const;
  float GetMaxHealth() const;
  Rectangle GetRect() const;

  void SetPosition(Vector2 pos);
  void SetVelocity(Vector2 vel);
  void SetPlayerReference(Vector2 *player_pos);

  // Utility methods
  float GetDistanceToPlayer() const;
  bool IsPlayerInRange(float range) const;
  void Stun(float duration = 2.0f);
  void DealDamage(float damage);
  bool IsInvulnerable() const;
  bool IsFacingLeft() const;
  void SetFacing(bool left);

  // Attack methods for AI
  bool CanAttack() const;
  void EnterAttackMode();
  void PerformAttack();
  void OnProjectileHit(Vector2 projectile_pos, int damage = 1);

  // Safe wrapper methods for collision handling
  void SafeOnHitFromAbove();
  void SafeOnHitFromSide();
  bool IsHitFromAbove(const Rectangle &characterRect) const;
  void CheckAndHandleWallCollision(); // New method for wall collision detection

  // Legacy compatibility methods
  bool CheckCollision(Rectangle other) const;
  void Render(
    Texture &tex, const std::unordered_map<int, Rectangle> &sprite_rects) const;
};
