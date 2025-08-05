#include <raylib.h>

#include "include/characters/character.hpp"
#include "include/characters/projectile_pool.hpp"
#include "include/collision/collision_handler.hpp"
#include "include/core/command.hpp"

Command::Command(Character *character)
    : character_(character), fireball_active_(false), projectile_pool_(nullptr), 
      show_instructions_(false), instructions_initialized_(false) {
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

void Command::SetCharacter(Character *character) {
  character_ = character;
}

Character *Command::GetActiveCharacter() const {
  return character_;
}

void Command::InitializeInstructionsButton() {
  if (!instructions_initialized_) {
    instructions_button_ = Button(
      "Instructions",
      [this]() {
        ToggleInstructions();
      },
      Rectangle{0.0f, 0.0f, 16.0f, 16.0f}, 
      Rectangle{static_cast<float>(GetScreenWidth() - 120), 10.0f, 100.0f, 40.0f}, "res/sprites/icons/icons.png");
    instructions_initialized_ = true;
  }
}

void Command::HandleInput() {
  if (!character_)
    return;

  if (!instructions_initialized_) {
    InitializeInstructionsButton();
  }

  UpdateInstructionsButton();

  if (IsKeyPressed(KEY_I)) {
    ToggleInstructions();
  }

  if (show_instructions_) {
    return;
  }

  if (fireball_active_ && projectile_pool_) {
    bool has_active_projectile = false;
   
    has_active_projectile = projectile_pool_->HasActiveMarioFireball() || 
                           projectile_pool_->HasActiveElectricBall();
    
    if (!has_active_projectile) {
      fireball_active_ = false;
    }
  }


if (IsKeyDown(KEY_LEFT)) {
    MoveCharacter(true);
  } else if (IsKeyDown(KEY_RIGHT)) {
    MoveCharacter(false);
  }
 

  if (IsKeyDown(KEY_UP)) {
    JumpCharacter();
  }

  // Character abilities
  if (IsKeyDown(KEY_E)) {
    character_->Evolve();
  }

  if (IsKeyDown(KEY_S)) {
    character_->ToStarman();
  }

  // --- CHEAT: Character Invincibility (K Key) ---
  if (IsKeyDown(KEY_K)) {
    character_->ToStarman();
  }

  // --- Handle Projectile Shooting (Shift Key) ---
  if (IsKeyDown(KEY_LEFT_SHIFT) && character_->GetState() == 2 && !fireball_active_) {
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

  DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), ColorAlpha(BLACK, 0.7f));
  
  Rectangle panelRect = {
    static_cast<float>(GetScreenWidth() / 2 - 400), 
    static_cast<float>(GetScreenHeight() / 2 - 300),
    800.0f, 600.0f
  };
  DrawRectangleRec(panelRect, ColorAlpha(WHITE, 0.95f));
  DrawRectangleLinesEx(panelRect, 3, DARKBLUE);
  
  const char* character_name = "CHARACTER";
  if (character_) {
    CharacterType char_type = character_->GetCharacter();
    character_name = (char_type == CharacterType::MARIO) ? "MARIO" : 
                    (char_type == CharacterType::LUIGI) ? "LUIGI" : "CHARACTER";
  }
  
  char title[100];
  snprintf(title, sizeof(title), "SUPER %s CONTROLS", character_name);
  
  int titleX = GetScreenWidth() / 2 - MeasureText(title, 28) / 2;
  DrawText(title, titleX, GetScreenHeight() / 2 - 280, 28, DARKBLUE);
  
  int startY = GetScreenHeight() / 2 - 230;
  int lineSpacing = 25;
  int leftCol = GetScreenWidth() / 2 - 380;
  int rightCol = GetScreenWidth() / 2 + 20;
  
  DrawText("=== BASIC CONTROLS ===", leftCol, startY, 16, DARKGREEN);
  DrawText("LEFT/RIGHT Arrows: Move", leftCol, startY + lineSpacing * 1, 12, BLACK);
  DrawText("UP Arrow: Jump", leftCol, startY + lineSpacing * 2, 12, BLACK);
  
  if (character_) {
    CharacterType char_type = character_->GetCharacter();
    if (char_type == CharacterType::MARIO) {
      DrawText("SHIFT: Shoot Fireball (Fire State only)", leftCol, startY + lineSpacing * 3, 12, BLACK);
    } else if (char_type == CharacterType::LUIGI) {
      DrawText("SHIFT: Shoot Electric Ball (Fire State only)", leftCol, startY + lineSpacing * 3, 12, BLACK);
    } else {
      DrawText("SHIFT: Shoot Projectile (Fire State only)", leftCol, startY + lineSpacing * 3, 12, BLACK);
    }
  }
  DrawText("I: Toggle this help screen", leftCol, startY + lineSpacing * 4, 12, BLACK);
  
  DrawText("=== CHARACTER ABILITIES ===", leftCol, startY + lineSpacing * 6, 16, MAROON);
  DrawText("E: Evolve Character", leftCol, startY + lineSpacing * 7, 12, BLACK);
  DrawText("S: Activate Star Power", leftCol, startY + lineSpacing * 8, 12, BLACK);
  
  DrawText("=== GAME CONTROLS ===", leftCol, startY + lineSpacing * 10, 16, PURPLE);
  DrawText("ESC: Exit to Menu", leftCol, startY + lineSpacing * 11, 12, BLACK);
  
  DrawText("=== CHEAT CODES ===", leftCol, startY + lineSpacing * 13, 16, RED);
  DrawText("K: Character Invincibility (Star Power)", leftCol, startY + lineSpacing * 14, 12, RED);
  
  DrawText("=== GAMEPLAY TIPS ===", rightCol, startY, 16, DARKGREEN);
  DrawText("• Jump on enemies to defeat them", rightCol, startY + lineSpacing * 1, 12, BLACK);
  DrawText("• Avoid touching enemies from the side", rightCol, startY + lineSpacing * 2, 12, BLACK);
  DrawText("• Hit blocks from below to get items", rightCol, startY + lineSpacing * 3, 12, BLACK);
  DrawText("• Collect power-ups to grow stronger", rightCol, startY + lineSpacing * 4, 12, BLACK);
  
  DrawText("=== CHARACTER STATES ===", rightCol, startY + lineSpacing * 6, 16, MAROON);
  DrawText("• Small: Default state", rightCol, startY + lineSpacing * 7, 12, BLACK);
  DrawText("• Super: Can break blocks", rightCol, startY + lineSpacing * 8, 12, BLACK);
  DrawText("• Fire: Shoots projectiles", rightCol, startY + lineSpacing * 9, 12, BLACK);
  DrawText("• Star: Temporary invincibility", rightCol, startY + lineSpacing * 10, 12, BLACK);
  
  DrawText("=== ENEMY TYPES ===", rightCol, startY + lineSpacing * 12, 16, PURPLE);
  DrawText("• Goomba: Brown mushroom enemies", rightCol, startY + lineSpacing * 13, 12, BLACK);
  DrawText("• Koopa: Green turtle enemies", rightCol, startY + lineSpacing * 14, 12, BLACK);
  DrawText("• Piranha Plant: Dangerous pipe plants", rightCol, startY + lineSpacing * 15, 12, BLACK);
  DrawText("• Bowser: Final boss with multiple hits", rightCol, startY + lineSpacing * 16, 12, BLACK);
  
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
  if (character_) {
    character_->Run(left);
  }
}

void Command::StopCharacter() {
  if (character_) {
    character_->StopX();
  }
}

void Command::JumpCharacter() {
  if (character_) {
    character_->Jump();
  }
}

void Command::ShootFireball() {
  if (!character_ || !projectile_pool_) {
    return;
  }

  Rectangle character_rect = character_->GetRectangle();
  bool to_left = character_->IsToLeft();
  
  Vector2 spawn_position = {
    character_rect.x + (to_left ? -10.0f : character_rect.width + 10.0f),
    character_rect.y + character_rect.height / 2.0f
  };

  CharacterType char_type = character_->GetCharacter();
  if (char_type == CharacterType::MARIO) {
    projectile_pool_->ShootMarioFireball(spawn_position, to_left);
  } else if (char_type == CharacterType::LUIGI) {
    projectile_pool_->ShootElectricBall(spawn_position, to_left);
  }
}

void Command::SetFireballActive(bool active) {
  fireball_active_ = active;
}

bool Command::IsFireballActive() const {
  return fireball_active_;
}