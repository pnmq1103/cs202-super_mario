#include <raylib.h>

#include "include/characters/character.hpp"
#include "include/characters/projectile_pool.hpp"
#include "include/collision/collision_handler.hpp"
#include "include/core/command.hpp"

Command::Command(Character *mario, Character *luigi)
    : mario_(mario), luigi_(luigi), fireball_active_(false), projectile_pool_(nullptr), 
      show_instructions_(false), instructions_initialized_(false) {
  active_character_ = mario_ ? mario_ : luigi_;
}

Command::~Command() {
  if (projectile_pool_) {
    delete projectile_pool_;
    projectile_pool_ = nullptr;
  }
}

void Command::InitializeProjectilePool(CollisionHandler* collision_handler) {
  if (!projectile_pool_ && collision_handler) {
    projectile_pool_ = new ProjectilePool(collision_handler);
  }
}

void Command::UpdateProjectiles() {
  if (projectile_pool_) {
    projectile_pool_->SetFrameCount();
    projectile_pool_->Update();
  }
}

void Command::DrawProjectiles() {
  if (projectile_pool_) {
    projectile_pool_->Draw();
  }
}

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

void Command::SetActiveCharacter(Character *character) {
  active_character_ = character;
}

Character *Command::GetActiveCharacter() const {
  return active_character_;
}

void Command::InitializeInstructionsButton() {
  if (!instructions_initialized_) {
    instructions_button_ = Button(
      "Instructions",
      [this]() {
        ToggleInstructions();
      },
      Rectangle{0.0f, 0.0f, 16.0f, 16.0f}, // source rectangle
      Rectangle{static_cast<float>(GetScreenWidth() - 120), 10.0f, 100.0f, 40.0f}, // destination rectangle
      "res/sprites/icons/icons.png" // icon texture
    );
    instructions_initialized_ = true;
  }
}

void Command::HandleInput() {
  if (!active_character_)
    return;

  // Initialize instructions button if needed
  if (!instructions_initialized_) {
    InitializeInstructionsButton();
  }

  // Update instructions button
  UpdateInstructionsButton();

  // Handle instructions toggle with I key
  if (IsKeyPressed(KEY_I)) {
    ToggleInstructions();
  }

  // Don't process other inputs when instructions are showing
  if (show_instructions_) {
    return;
  }

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

  if (IsKeyPressed(KEY_TAB)) {
    SwitchCharacter();
  }

  
  bool is_moving = false;
  if (IsKeyDown(KEY_LEFT)) {
    MoveCharacter(true); 
    is_moving = true;
  }
  if (IsKeyDown(KEY_RIGHT)) {
    MoveCharacter(false); 
    is_moving = true;
  }
  if (!is_moving) {
    StopCharacter();
  }

  if (IsKeyPressed(KEY_UP)) {
    JumpCharacter();
  }

  if (IsKeyPressed(KEY_E)) {
    active_character_->Evolve();
  }

  if (IsKeyPressed(KEY_S)) {
    active_character_->ToStarman();
  }

  // --- CHEAT: Mario Invincibility (K Key) ---
  if (IsKeyPressed(KEY_K)) {
    if (mario_) {
      mario_->ToStarman();
    }
  }

  // --- Handle Fireball (Shift Key) ---
  if (IsKeyPressed(KEY_LEFT_SHIFT) && active_character_->GetState() == 2 && !fireball_active_) {
    ShootFireball();
    fireball_active_ = true;
  }
}

void Command::UpdateInstructionsButton() {
  if (instructions_initialized_) {
    instructions_button_.Update();
  }
}

void Command::DrawInstructionsButton() {
  if (instructions_initialized_) {
    instructions_button_.Draw();
    DrawText("Help", GetScreenWidth() - 115, 20, 12, BLACK);
  }
}

void Command::DrawInstructionsPanel() {
  if (!show_instructions_) return;

  // Semi-transparent background overlay
  DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), ColorAlpha(BLACK, 0.7f));
  
  // Instructions panel background
  Rectangle panelRect = {
    static_cast<float>(GetScreenWidth() / 2 - 400), 
    static_cast<float>(GetScreenHeight() / 2 - 300),
    800.0f, 600.0f
  };
  DrawRectangleRec(panelRect, ColorAlpha(WHITE, 0.95f));
  DrawRectangleLinesEx(panelRect, 3, DARKBLUE);
  
  // Instructions title
  int titleX = GetScreenWidth() / 2 - MeasureText("SUPER MARIO CONTROLS", 28) / 2;
  DrawText("SUPER MARIO CONTROLS", titleX, GetScreenHeight() / 2 - 280, 28, DARKBLUE);
  
  // Instructions content
  int startY = GetScreenHeight() / 2 - 230;
  int lineSpacing = 25;
  int leftCol = GetScreenWidth() / 2 - 380;
  int rightCol = GetScreenWidth() / 2 + 20;
  

  DrawText("=== BASIC CONTROLS ===", leftCol, startY, 16, DARKGREEN);
  DrawText("LEFT/RIGHT Arrows: Move", leftCol, startY + lineSpacing * 1, 12, BLACK);
  DrawText("UP Arrow: Jump", leftCol, startY + lineSpacing * 2, 12, BLACK);
  DrawText("TAB: Switch Character (Mario/Luigi)", leftCol, startY + lineSpacing * 3, 12, BLACK);
  DrawText("SHIFT: Shoot Fireball (Fire State only)", leftCol, startY + lineSpacing * 4, 12, BLACK);
  DrawText("I: Toggle this help screen", leftCol, startY + lineSpacing * 5, 12, BLACK);
  
  DrawText("=== CHARACTER ABILITIES ===", leftCol, startY + lineSpacing * 7, 16, MAROON);
  DrawText("E: Evolve Character", leftCol, startY + lineSpacing * 8, 12, BLACK);
  DrawText("S: Activate Star Power", leftCol, startY + lineSpacing * 9, 12, BLACK);
  
  DrawText("=== GAME CONTROLS ===", leftCol, startY + lineSpacing * 11, 16, PURPLE);
  DrawText("ESC: Exit to Menu", leftCol, startY + lineSpacing * 12, 12, BLACK);
  
  DrawText("=== CHEAT CODES ===", leftCol, startY + lineSpacing * 14, 16, RED);
  DrawText("K: Mario Invincibility (Star Power)", leftCol, startY + lineSpacing * 15, 12, RED);
  
  // Right column - Gameplay Tips
  DrawText("=== GAMEPLAY TIPS ===", rightCol, startY, 16, DARKGREEN);
  DrawText("• Jump on enemies to defeat them", rightCol, startY + lineSpacing * 1, 12, BLACK);
  DrawText("• Avoid touching enemies from the side", rightCol, startY + lineSpacing * 2, 12, BLACK);
  DrawText("• Hit blocks from below to get items", rightCol, startY + lineSpacing * 3, 12, BLACK);
  DrawText("• Collect power-ups to grow stronger", rightCol, startY + lineSpacing * 4, 12, BLACK);
  DrawText("• Use TAB to switch between characters", rightCol, startY + lineSpacing * 5, 12, BLACK);
  
  DrawText("=== CHARACTER STATES ===", rightCol, startY + lineSpacing * 7, 16, MAROON);
  DrawText("• Small: Default state", rightCol, startY + lineSpacing * 8, 12, BLACK);
  DrawText("• Super: Can break blocks", rightCol, startY + lineSpacing * 9, 12, BLACK);
  DrawText("• Fire: Shoots fireballs", rightCol, startY + lineSpacing * 10, 12, BLACK);
  DrawText("• Star: Temporary invincibility", rightCol, startY + lineSpacing * 11, 12, BLACK);
  
  DrawText("=== ENEMY TYPES ===", rightCol, startY + lineSpacing * 13, 16, PURPLE);
  DrawText("• Goomba: Brown mushroom enemies", rightCol, startY + lineSpacing * 14, 12, BLACK);
  DrawText("• Koopa: Green turtle enemies", rightCol, startY + lineSpacing * 15, 12, BLACK);
  DrawText("• Piranha Plant: Dangerous pipe plants", rightCol, startY + lineSpacing * 16, 12, BLACK);
  DrawText("• Bowser: Final boss with multiple hits", rightCol, startY + lineSpacing * 17, 12, BLACK);
  
  // Close instructions note
  DrawText("Press 'I' key or click the Help button again to close", 
           GetScreenWidth() / 2 - MeasureText("Press 'I' key or click the Help button again to close", 14) / 2,
           GetScreenHeight() / 2 + 250, 14, DARKGRAY);
}

bool Command::IsShowingInstructions() const {
  return show_instructions_;
}

void Command::ToggleInstructions() {
  show_instructions_ = !show_instructions_;
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

void Command::SwitchCharacter() {
  if (active_character_ == mario_ && luigi_) {
    active_character_ = luigi_;
  } else if (active_character_ == luigi_ && mario_) {
    active_character_ = mario_;
  }
}