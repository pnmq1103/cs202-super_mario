#include "include/enemies/bowser.hpp"
#include "include/characters/projectile_pool.hpp" // Add this include for ProjectilePool
#include "include/enemies/movement_strategy.hpp"
#include "include/managers/enemy_manager.hpp"

Bowser::Bowser(Vector2 pos, float Nscale)
    : Enemy(pos, Nscale, EnemyType::Bowser), hp(3), fire_timer(0.0f),
      attack_cooldown(2.0f), rage_mode(false) {
  // Set up basic walking movement like Goomba/Koopa
  SetMovementStrategy(new WalkingMovement(30.0f, false)); // Start moving right

  // Bowser properties - more health than regular enemies
  health          = 3.0f; // 3 hits to defeat
  max_health      = 3.0f;
  detection_range = 150.0f; // Detection range for fireball shooting

  // Set initial velocity for walking
  velocity.x  = 30.0f; // Start walking right, like Goomba
  facing_left = false;

  pos.y -= scale * 16;

  std::cout << "Bowser spawned with initial velocity: " << velocity.x
            << std::endl;
}

Bowser::~Bowser() {}

void Bowser::OnHit() {
  // Simple hit handling like Goomba, but with more health
  hp--;
  DealDamage(1.0f);

  if (hp <= 0) {
    alive    = false;
    state    = EnemyState::Dead;
    velocity = {0.0f, 0.0f};
    std::cout << "Bowser defeated!" << std::endl;
  } else {
    // Brief invulnerability after being hit
    invulnerability_timer = 1.0f;

    // Enter rage mode when low on health (faster shooting)
    if (hp <= 1 && !rage_mode) {
      EnterRageMode();
    }

    std::cout << "Bowser hit! HP remaining: " << hp << std::endl;
  }
}

void Bowser::Update() {
  if (!alive)
    return;

  // Update fire timer
  fire_timer += GetFrameTime();

  // Simple fireball shooting when player is close
  if (player_position && IsPlayerInRange(detection_range)) {
    // Face the player
    bool should_face_left = (player_position->x < position.x);
    SetFacing(should_face_left);

    // Shoot fireball periodically when player is close
    if (fire_timer >= attack_cooldown && state != EnemyState::Stunned) {
      ShootFireball();
      fire_timer = 0.0f;
    }
  }

  // Simple update like Goomba - just walk and let movement strategy handle
  // direction
  if (state == EnemyState::Normal && GetMovementStrategy()) {
    // Basic walking behavior is handled by movement strategy
  }

  // Call base enemy update (handles physics, animation, movement)
  Enemy::Update();
}

void Bowser::ShootFireball() {
  try {
    // Get ProjectilePool from EnemyManager with null checking
    ProjectilePool *pool = EnemyManager::GetProjectilePool();
    if (pool != nullptr) {
      // Calculate fireball spawn position
      Vector2 spawn_position = {
        position.x + (facing_left ? -10.0f : frame.width * scale + 10.0f),
        position.y + frame.height * scale / 2.0f // Center height
      };

      // Shoot enemy fireball
      pool->ShootEnemyFireball(spawn_position, facing_left);

      std::cout << "Bowser shoots fireball!" << std::endl;
    } else {
      std::cout << "Warning: ProjectilePool is null, cannot shoot fireball!"
                << std::endl;
    }
  } catch (const std::exception &e) {
    std::cerr << "Error in Bowser::ShootFireball(): " << e.what() << std::endl;
  }
}

void Bowser::EnterRageMode() {
  // Simplified rage mode - just faster fireball shooting
  rage_mode       = true;
  attack_cooldown = 1.0f;
  std::cout << "Bowser enters rage mode!" << std::endl;
}

void Bowser::OnHitFromAbove() {
  try {
    // Mario jumped on Bowser - similar to Goomba but with more health
    OnHit();
  } catch (const std::exception &e) {
    std::cerr << "Error in Bowser::OnHitFromAbove(): " << e.what() << std::endl;
    // Fallback: mark as dead
    alive    = false;
    state    = EnemyState::Dead;
    velocity = {0.0f, 0.0f};
  }
}

void Bowser::OnHitFromSide() {
  try {
    // Side collision or projectile hit - similar to Goomba but with more health
    if (IsInvulnerable()) {
      return; // No damage if invulnerable
    }

    OnHit();
  } catch (const std::exception &e) {
    std::cerr << "Error in Bowser::OnHitFromSide(): " << e.what() << std::endl;
    // Fallback: mark as dead
    alive    = false;
    state    = EnemyState::Dead;
    velocity = {0.0f, 0.0f};
  }
}

EnemyType Bowser::GetType() {
  return EnemyType::Bowser;
}

Enemy *Bowser::Clone() const {
  Bowser *clone = new Bowser(*this);
  // Clone the movement strategy
  if (movement_strategy_) {
    clone->SetMovementStrategy(new WalkingMovement(30.0f, facing_left));
  }
  return clone;
}

int Bowser::GetHP() const {
  return hp;
}

bool Bowser::IsInRageMode() const {
  return rage_mode;
}