/*#include "include/core/application.hpp"
#include "include/core/application.hpp"
#include <raylib.h>

int main() {
  App.Init();

  while (!WindowShouldClose() && !App.ShouldClose()) {
    App.Update();
    BeginDrawing();
    ClearBackground(RAYWHITE);
    App.Draw();
    EndDrawing();
  }

  CloseWindow();
  return 0;
}*/

#include "include/characters/character.hpp"
#include "include/characters/projectile_pool.hpp"
#include "include/core/application.hpp"
#include "include/core/command.hpp"
#include "include/enemies/goomba.hpp"
#include "include/enemies/koopa_troopa.hpp"
#include "include/enemies/piranha_plant.hpp"
#include "include/enemies/bowser.hpp"
#include "include/managers/enemy_manager.hpp"
#include "include/objects/brick_block.hpp"
#include "include/objects/coin.hpp"
#include "include/objects/fire_flower.hpp"
#include "include/objects/object_manager.hpp"
#include "include/objects/pipe_block.hpp"
#include "include/objects/question_block.hpp"
#include "include/objects/static_block.hpp"
#include "include/objects/super_mushroom.hpp"
#include "include/objects/super_star.hpp"
#include <cmath>
#include <iostream>
#include <list>
#include <raylib.h>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
#include <memory>

int main() {
  InitWindow(1800, 1000, "Super Mario - Enemy System Test with Sprites");
  SetTargetFPS(60);
  App.Resource().Init();
  
  // Load enemy texture and set up sprite rectangles
  Texture enemyTexture = App.Resource().GetEnemy();
  std::unordered_map<int, Rectangle> enemySpriteRects;
  
  // Define sprite rectangles for each enemy type (adjust these based on your sprite sheet)
  // These are example coordinates - you'll need to adjust based on your actual sprite sheet GOOMBA (small enemies - 16x15/16 pixels)
  enemySpriteRects[0] = {0, 188, 16, 15}; // From ID 21: x=0, y=188, w=16, h=15
  enemySpriteRects[1] = {17, 187, 16, 16}; // From ID 22: x=17, y=187, w=16, h=16

  // KOOPA TROOPA (medium enemies - 16x22/24 pixels)
  enemySpriteRects[2] = {17, 241, 16, 22}; // From ID 27: x=17, y=241, w=16, h=22
  enemySpriteRects[3] = {34, 239, 16, 24}; // From ID 28: x=34, y=239, w=16, h=24
  enemySpriteRects[4] = {0, 205, 16, 16}; // Shell from ID 23: x=0, y=205, w=16, h=16

  // BOWSER (large enemy - 46x40/45x38 pixels)
  enemySpriteRects[7] = {0, 0, 46, 40};  // From ID 1: x=0, y=0, w=46, h=40
  enemySpriteRects[8] = {47, 2, 45, 38}; // From ID 2: x=47, y=2, w=45, h=38
  
  // Initialize Object Manager for blocks/items
  ObjectManager &object = ObjectManager::GetInstance();
  object.Reset(4);
  
  // Create extensive ground for enemies to walk on
  for (int i = 0; i < 50; i++) {
    object.AddStaticBlock({(float)(i * 32), 900}, 'g');
  }
  
  // Add some platforms for testing
  object.AddStaticBlock({300, 800}, 'g');
  object.AddStaticBlock({332, 800}, 'g');
  object.AddStaticBlock({364, 800}, 'g');
  object.AddStaticBlock({396, 800}, 'g');
  
  object.AddStaticBlock({600, 700}, 'g');
  object.AddStaticBlock({632, 700}, 'g');
  object.AddStaticBlock({664, 700}, 'g');
  
  // Add interactive blocks for testing
  object.AddQuestionBlock({500, 600}, super_mushroom);
  object.AddQuestionBlock({700, 500}, super_star);
  object.AddBrickBlock({600, 600});
  object.AddBrickBlock({632, 600});
  object.AddPipeBlock({800, 900}, 3, false, true, true);
  object.AddPipeBlock({1200, 900}, 2, false, true, true);
  
  // Create Mario and Luigi for testing character interactions
  Character mario(4.0f);
  mario.SetCharacter(MARIO, {100, 800});
  
  Character luigi(4.0f);
  luigi.SetCharacter(LUIGI, {100, 800});
  
  // Create command system for character control
  Command inputCommand(&mario, &luigi);
  ProjectilePool pool;
  inputCommand.SetProjectilePool(&pool);
  
  // Create Enemy Manager for comprehensive enemy testing
  std::unique_ptr<EnemyManager> enemyManager = std::make_unique<EnemyManager>();
  
  // Set up character references for enemy AI
  Rectangle charRect = mario.GetRectangle();
  Vector2 marioPos = {charRect.x, charRect.y};
  Vector2 luigiPos = {charRect.x, charRect.y};
  Vector2 activePos = {charRect.x, charRect.y};
  enemyManager->SetCharacterReferences(&marioPos, &luigiPos, &activePos);
  
  // Spawn different types of enemies for testing
  enemyManager->SpawnEnemy(EnemyType::Goomba, {200.0f, 870.0f});    // Ground Goomba
  enemyManager->SpawnEnemy(EnemyType::Goomba, {400.0f, 870.0f});    // Another Goomba
  enemyManager->SpawnEnemy(EnemyType::Koopa, {300.0f, 870.0f});     // Ground Koopa
  enemyManager->SpawnEnemy(EnemyType::Koopa, {350.0f, 770.0f});     // Platform Koopa
  enemyManager->SpawnEnemy(EnemyType::Piranha, {820.0f, 870.0f});   // Piranha in pipe
  enemyManager->SpawnEnemy(EnemyType::Piranha, {1220.0f, 870.0f});  // Another Piranha
  
  // Test variables
  int testMode = 0; // 0=Normal, 1=Spawn Test, 2=Boss Test, 3=Formation Test
  float spawnTimer = 0.0f;
  bool showDebugInfo = true;
  bool useSprites = true; // Toggle for sprite vs rectangle rendering
  int points = 0;
  int enemiesDefeated = 0;
  float animationTimer = 0.0f; // For sprite animation
  
  // Camera for better viewing
  Camera2D camera = { 0 };
  camera.target = {mario.GetRectangle().x, mario.GetRectangle().y};
  camera.offset = {GetScreenWidth()/2.0f, GetScreenHeight()/2.0f};
  camera.rotation = 0.0f;
  camera.zoom = 1.0f;
  
  while (!WindowShouldClose()) {
    float deltaTime = GetFrameTime();
    spawnTimer += deltaTime;
    animationTimer += deltaTime;
    
    // Handle input
    inputCommand.HandleInput();
    Character* activeCharacter = inputCommand.GetActiveCharacter();
    
    // Update character positions for enemy AI
    if (activeCharacter) {
      Rectangle activeRect = activeCharacter->GetRectangle();
      activePos = {activeRect.x, activeRect.y};
      
      if (activeCharacter == &mario) {
        marioPos = activePos;
      } else {
        luigiPos = activePos;
      }
      
      // Update camera to follow character smoothly
      camera.target.x += (activeRect.x - camera.target.x) * 0.1f;
      camera.target.y += ((activeRect.y - 100) - camera.target.y) * 0.1f;
    }
    
    // Enemy Test Controls
    if (IsKeyPressed(KEY_ONE)) {
      testMode = 0;
    }
    if (IsKeyPressed(KEY_TWO)) {
      testMode = 1;
    }
    if (IsKeyPressed(KEY_THREE)) {
      testMode = 2;
      // Spawn boss for testing
      enemyManager->SpawnBoss(EnemyType::Bowser, {600.0f, 870.0f});
    }
    if (IsKeyPressed(KEY_FOUR)) {
      testMode = 3;
      // Spawn formation
      enemyManager->SpawnPatrolGroup(EnemyType::Goomba, {150.0f, 870.0f}, {350.0f, 870.0f}, 3);
    }
    
    // Toggle sprite rendering
    if (IsKeyPressed(KEY_SPACE)) {
      useSprites = !useSprites;
    }
    
    // Test enemy spawning in spawn mode
    if (testMode == 1 && spawnTimer >= 2.0f) {
      // Spawn random enemy every 2 seconds
      EnemyType types[] = {EnemyType::Goomba, EnemyType::Koopa, EnemyType::Piranha};
      EnemyType randomType = types[rand() % 3];
      float randomX = 200.0f + (rand() % 800);
      enemyManager->SpawnEnemy(randomType, {randomX, 870.0f});
      spawnTimer = 0.0f;
    }
    
    // Difficulty controls
    if (IsKeyPressed(KEY_Q)) {
      enemyManager->SetDifficulty(0.5f); // Easy mode
    }
    if (IsKeyPressed(KEY_W)) {
      enemyManager->SetDifficulty(1.0f); // Normal mode
    }
    if (IsKeyPressed(KEY_E)) {
      enemyManager->SetDifficulty(2.0f); // Hard mode
    }
    
    // Enemy control tests
    if (IsKeyPressed(KEY_P)) {
      enemyManager->PauseAllEnemies();
    }
    if (IsKeyPressed(KEY_R)) {
      enemyManager->ResumeAllEnemies();
    }
    if (IsKeyPressed(KEY_C)) {
      enemyManager->ClearAllEnemies();
      enemiesDefeated = 0;
      points = 0;
    }
    if (IsKeyPressed(KEY_B)) {
      enemyManager->SetBossRageMode();
    }
    
    // Toggle debug info
    if (IsKeyPressed(KEY_F1)) {
      showDebugInfo = !showDebugInfo;
    }
    
    // Character evolution for testing
    if (IsKeyPressed(KEY_G)) {
      if (activeCharacter) activeCharacter->Evolve();
    }
    if (IsKeyPressed(KEY_H)) {
      if (activeCharacter) activeCharacter->Die();
    }
    if (IsKeyPressed(KEY_S)) {
      if (activeCharacter) activeCharacter->ToStarman();
    }
    
    // Ground collision for character
    if (activeCharacter) {
      Rectangle rect = activeCharacter->GetRectangle();
      if (rect.y + rect.height >= 900) {
        activeCharacter->StopY(900);
      }
      
      // Simple platform collision
      if (rect.y + rect.height >= 800 && rect.y + rect.height <= 820 && 
          rect.x + rect.width >= 300 && rect.x <= 428) {
        activeCharacter->StopY(800);
      }
      if (rect.y + rect.height >= 700 && rect.y + rect.height <= 720 && 
          rect.x + rect.width >= 600 && rect.x <= 696) {
        activeCharacter->StopY(700);
      }
      
      // Update character
      activeCharacter->SetFrameCount();
      activeCharacter->Update();
    }
    
    // Update systems
    object.SetFrameCount();
    object.Update();
    
    // Update projectiles
    pool.SetFrameCount();
    pool.Update();
    
    // Update enemy manager
    if (enemyManager) {
      size_t enemiesBefore = enemyManager->GetAliveEnemyCount();
      
      enemyManager->UpdateAll(deltaTime);
      if (activeCharacter) {
        enemyManager->HandleCharacterInteractions(activeCharacter);
      }
      enemyManager->HandleWallCollisions();
      enemyManager->ClearDeadEnemies();
      
      // Count defeated enemies
      size_t enemiesAfter = enemyManager->GetAliveEnemyCount();
      if (enemiesBefore > enemiesAfter) {
        enemiesDefeated += (int)(enemiesBefore - enemiesAfter);
        points += (int)(enemiesBefore - enemiesAfter) * 100;
      }
    }
    
    // Test block hitting
    if (IsKeyDown(KEY_A)) {
      object.OnHit(1);
    }
    
    // === DRAWING ===
    BeginDrawing();
    ClearBackground(SKYBLUE);
    
    BeginMode2D(camera);
    
    // Draw objects and blocks
    object.Draw();
    
    // Draw character
    if (activeCharacter) {
      activeCharacter->Draw();
    }
    
    // Draw projectiles
    pool.Draw();
    
    // Draw enemies with sprite support
    if (enemyManager) {
      for (Enemy* enemy : enemyManager->GetEnemies()) {
        if (enemy && enemy->IsAlive()) {
          Rectangle rect = enemy->GetRect();
          
          if (useSprites) {
            // Enhanced sprite rendering
            int spriteIndex = 0;
            int animFrame = ((int)(animationTimer * 4)) % 2; // 4 fps animation, 2 frames
            
            // Determine sprite index based on enemy type and state
            switch (enemy->GetType()) {
              case EnemyType::Goomba:
                spriteIndex = animFrame; // Alternates between 0 and 1
                break;
              case EnemyType::Koopa:
                if (enemy->GetState() == EnemyState::Shell) {
                  spriteIndex = 4; // Shell sprite
                } else {
                  spriteIndex = 2 + animFrame; // Walking animation (2 and 3)
                }
                break;
              case EnemyType::Piranha:
                spriteIndex = 5 + animFrame; // Piranha animation (5 and 6)
                break;
              case EnemyType::Bowser:
                if (enemy->GetState() == EnemyState::Attacking) {
                  spriteIndex = 9; // Attacking frame
                } else {
                  spriteIndex = 7 + animFrame; // Normal animation (7 and 8)
                }
                break;
            }
            
            // Get the sprite rectangle
            Rectangle sourceRect = enemySpriteRects[spriteIndex];
            Rectangle destRect = rect;
            
            // Apply visual effects based on state
            Color tint = WHITE;
            switch (enemy->GetState()) {
              case EnemyState::Stunned:
                tint = ColorAlpha(WHITE, 0.7f);
                break;
              case EnemyState::Attacking:
                tint = ColorBrightness(WHITE, 0.3f);
                break;
              default:
                break;
            }
            
            // Apply invulnerability flashing
            if (enemy->IsInvulnerable()) {
              tint = ColorAlpha(tint, 0.5f);
            }
            
            // Flip sprite if enemy is facing left
            if (enemy->IsFacingLeft()) {
              sourceRect.width = -sourceRect.width;
            }
            
            // Draw the sprite
            DrawTexturePro(enemyTexture, sourceRect, destRect, {0, 0}, 0.0f, tint);
            
          } else {
            // Fallback to colored rectangles
            Color color = RED;
            
            // Color based on enemy type
            switch (enemy->GetType()) {
              case EnemyType::Goomba:
                color = BROWN;
                break;
              case EnemyType::Koopa:
                color = GREEN;
                break;
              case EnemyType::Piranha:
                color = DARKGREEN;
                break;
              case EnemyType::Bowser:
                color = ORANGE;
                rect.width *= 1.5f;
                rect.height *= 1.5f;
                break;
            }
            
            // Visual effects based on state
            switch (enemy->GetState()) {
              case EnemyState::Stunned:
                color = ColorAlpha(color, 0.7f);
                break;
              case EnemyState::Attacking:
                color = ColorBrightness(color, 0.3f);
                break;
              case EnemyState::Shell:
                color = GRAY;
                break;
              default:
                break;
            }
            
            // Apply invulnerability flashing
            if (enemy->IsInvulnerable()) {
              color = ColorAlpha(color, 0.5f);
            }
            
            DrawRectangleRec(rect, color);
            DrawRectangleLinesEx(rect, 1.0f, BLACK);
            
            // Draw type indicator
            const char* typeName = "";
            switch (enemy->GetType()) {
              case EnemyType::Goomba: typeName = "G"; break;
              case EnemyType::Koopa: typeName = "K"; break;
              case EnemyType::Piranha: typeName = "P"; break;
              case EnemyType::Bowser: typeName = "B"; break;
            }
            DrawText(typeName, (int)rect.x + 2, (int)rect.y + 2, 12, WHITE);
          }
          
          // Draw state indicators (always visible)
          if (enemy->GetState() == EnemyState::Stunned) {
            DrawText("!", (int)rect.x + (int)rect.width - 8, (int)rect.y - 10, 12, YELLOW);
          } else if (enemy->GetState() == EnemyState::Attacking) {
            DrawText("*", (int)rect.x + (int)rect.width - 8, (int)rect.y - 10, 12, RED);
          }
          
          // Draw health bar for bosses
          if (enemy->GetType() == EnemyType::Bowser) {
            float healthPercentage = enemy->GetHealth() / enemy->GetMaxHealth();
            Rectangle healthBar = {rect.x, rect.y - 20, rect.width, 6};
            DrawRectangleRec(healthBar, RED);
            healthBar.width *= healthPercentage;
            DrawRectangleRec(healthBar, GREEN);
            DrawText(TextFormat("HP: %.0f", enemy->GetHealth()), 
                    (int)rect.x, (int)rect.y - 35, 10, WHITE);
          }
          
          // Debug info
          if (showDebugInfo) {
            Vector2 velocity = enemy->GetVelocity();
            DrawText(TextFormat("V:%.0f,%.0f", velocity.x, velocity.y), 
                    (int)rect.x, (int)rect.y - 25, 8, WHITE);
            float distToPlayer = enemy->GetDistanceToPlayer();
            DrawText(TextFormat("D:%.0f", distToPlayer), 
                    (int)rect.x, (int)rect.y - 15, 8, YELLOW);
          }
        }
      }
    }
    
    EndMode2D();
    
    // === UI OVERLAY ===
    
    // Character info
    if (activeCharacter) {
      const char* activeCharName = (activeCharacter == &mario) ? "Mario" : "Luigi";
      DrawText(TextFormat("Character: %s", activeCharName), 10, 10, 20, WHITE);
      
      const char* stateNames[] = {"Small", "Big", "Fire/Electric"};
      DrawText(TextFormat("State: %s", stateNames[activeCharacter->GetState()]), 10, 35, 16, WHITE);
      
      if (activeCharacter->IsStarman()) {
        DrawText("STAR POWER!", 10, 55, 16, GOLD);
      }
      
      if (inputCommand.IsFireballActive()) {
        DrawText("FIREBALL ACTIVE!", 10, 75, 14, RED);
      }
    }
    
    // Game stats
    DrawText(TextFormat("Points: %d", points), 10, 100, 18, YELLOW);
    DrawText(TextFormat("Enemies Defeated: %d", enemiesDefeated), 10, 120, 16, LIME);
    
    // Enemy stats
    if (enemyManager) {
      size_t aliveEnemies = enemyManager->GetAliveEnemyCount();
      size_t totalEnemies = enemyManager->GetEnemyCount();
      DrawText(TextFormat("Enemies Alive: %zu/%zu", aliveEnemies, totalEnemies), 10, 145, 16, WHITE);
      
      if (enemyManager->HasBoss()) {
        Enemy* boss = enemyManager->GetBoss();
        if (boss) {
          float healthPercentage = boss->GetHealth() / boss->GetMaxHealth();
          DrawText(TextFormat("Boss HP: %.0f%%", healthPercentage * 100), 10, 165, 18, RED);
        }
      }
    }
    
    // Test mode indicator
    const char* modeNames[] = {"Normal", "Spawn Test", "Boss Test", "Formation Test"};
    DrawText(TextFormat("Mode: %s", modeNames[testMode]), 10, 190, 16, ORANGE);
    
    // Rendering mode indicator
    DrawText(TextFormat("Rendering: %s", useSprites ? "SPRITES" : "RECTANGLES"), 10, 210, 16, PURPLE);
    
    // Controls help
    DrawText("=== ENEMY TEST CONTROLS ===", 10, 240, 14, LIME);
    DrawText("Arrows: Move | UP: Jump | TAB: Switch Character", 10, 260, 12, WHITE);
    DrawText("SPACE: Toggle Sprites/Rectangles", 10, 275, 12, GRAY);
    DrawText("1-4: Test Modes | Q/W/E: Difficulty Easy/Normal/Hard", 10, 290, 12, WHITE);
    DrawText("P/R: Pause/Resume | C: Clear All | B: Boss Rage", 10, 305, 12, WHITE);
    DrawText("G: Evolve | H: Die | S: Star Power | A: Hit Blocks", 10, 320, 12, WHITE);
    DrawText("Shift: Fireball | F1: Debug Info", 10, 335, 12, WHITE);
    
    // Instructions
    DrawText("Jump on enemies to defeat them!", GetScreenWidth() - 280, 10, 16, LIME);
    DrawText("Avoid touching from the side!", GetScreenWidth() - 280, 30, 14, RED);
    DrawText("Use Star Power for invincibility!", GetScreenWidth() - 280, 50, 14, GOLD);
    DrawText(
      "Press SPACE to toggle sprites!", GetScreenWidth() - 280, 70, 14, GRAY);
    
    // Debug info
    if (showDebugInfo) {
      DrawText("=== DEBUG INFO ===", GetScreenWidth() - 200, 100, 12, ORANGE);
      DrawText(TextFormat("FPS: %d", GetFPS()), GetScreenWidth() - 200, 115, 10, WHITE);
      DrawText(TextFormat("Delta: %.3f", deltaTime), GetScreenWidth() - 200, 130, 10, WHITE);
      DrawText(TextFormat("Camera: %.0f,%.0f", camera.target.x, camera.target.y), GetScreenWidth() - 200, 145, 10, WHITE);
      DrawText(TextFormat("AnimTimer: %.2f", animationTimer), GetScreenWidth() - 200, 160, 10, WHITE);
      if (activeCharacter) {
        Rectangle rect = activeCharacter->GetRectangle();
        DrawText(TextFormat("Player: %.0f,%.0f", rect.x, rect.y), GetScreenWidth() - 200, 175, 10, WHITE);
        Vector2 speed = activeCharacter->GetSpeed();
        DrawText(TextFormat("Speed: %.0f,%.0f", speed.x, speed.y), GetScreenWidth() - 200, 190, 10, WHITE);
      }
    }
    
    EndDrawing();
  }
  
  CloseWindow();
  return 0;
}