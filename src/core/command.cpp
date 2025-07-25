#include <raylib.h>

#include "include/characters/character.hpp"
#include "include/characters/projectile_pool.hpp"
#include "include/core/command.hpp"

Command::Command(Character *mario, Character *luigi)
    : mario_(mario), luigi_(luigi), fireball_active_(false), projectile_pool_(nullptr) {
  active_character_ = mario_ ? mario_ : luigi_;
}

Command::~Command() {}

void Command::SetMario(Character *mario) {
  mario_ = mario;
  if (!active_character_)
    active_character_ = mario_;
}

void Command::SetLuigi(Character *luigi) {
  luigi_ = luigi;
  if (!active_character_)
    active_character_ = luigi_;
}

void Command::SetProjectilePool(ProjectilePool *projectile_pool) {
  projectile_pool_ = projectile_pool;
}

void Command::SetActiveCharacter(Character *character) {
  active_character_ = character;
}

Character *Command::GetActiveCharacter() const {
  return active_character_;
}

void Command::SwitchCharacter() {
  if (active_character_ == mario_ && luigi_) {
    active_character_ = luigi_;
  } else if (active_character_ == luigi_ && mario_) {
    active_character_ = mario_;
  }
}

void Command::HandleInput() {
  if (!active_character_)
    return;

  // Update fireball active state based on projectile pool
  if (fireball_active_ && projectile_pool_) {
    bool has_active_projectile = false;
    if (active_character_ == mario_) {
      has_active_projectile = projectile_pool_->HasActiveMarioFireball();
    } else if (active_character_ == luigi_) {
      has_active_projectile = projectile_pool_->HasActiveElectricBall();
    }
    
    if (!has_active_projectile) {
      fireball_active_ = false;
    }
  }

  // --- Handle Character Switch ---
  if (IsKeyPressed(KEY_TAB)) {
    SwitchCharacter();
  }

  // --- Handle Movement (ONLY Arrow Keys) ---
  bool is_moving = false;
  if (IsKeyDown(KEY_LEFT)) {
    MoveCharacter(true); // Move left
    is_moving = true;
  }
  if (IsKeyDown(KEY_RIGHT)) {
    MoveCharacter(false); // Move right
    is_moving = true;
  }
  if (!is_moving) {
    StopCharacter();
  }

  // --- Handle Jump (UP Arrow Key Only) ---
  if (IsKeyPressed(KEY_UP)) {
    JumpCharacter();
  }

  // --- Handle Fireball (Shift Key) ---
  if (IsKeyPressed(KEY_LEFT_SHIFT) && active_character_->GetState() == 2 && !fireball_active_) {
    ShootFireball();
    fireball_active_ = true;
  }
}

void Command::MoveCharacter(bool left) {
  if (active_character_) {
    active_character_->Run(left);
  }
}

void Command::StopCharacter() {
  if (active_character_) {
    active_character_->StopX();
  }
}

void Command::JumpCharacter() {
  if (active_character_) {
    active_character_->Jump();
  }
}

void Command::ShootFireball() {
  if (!active_character_ || !projectile_pool_) {
    return;
  }

  // Get character position and direction
  Rectangle character_rect = active_character_->GetRectangle();
  bool to_left = active_character_->IsToLeft();
  
  // Calculate projectile spawn position (slightly in front of character)
  Vector2 spawn_position = {
    character_rect.x + (to_left ? -10.0f : character_rect.width + 10.0f),
    character_rect.y + character_rect.height / 2.0f
  };

  // Determine character type and shoot appropriate projectile
  if (active_character_ == mario_) {
    // Mario shoots regular fireballs
    projectile_pool_->ShootMarioFireball(spawn_position, to_left);
  } else if (active_character_ == luigi_) {
    // Luigi shoots electric balls
    projectile_pool_->ShootElectricBall(spawn_position, to_left);
  }
}

void Command::SetFireballActive(bool active) {
  fireball_active_ = active;
}

bool Command::IsFireballActive() const {
  return fireball_active_;
}