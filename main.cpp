#include "include/characters/character.hpp"
#include "include/characters/projectile_pool.hpp"
#include "include/core/application.hpp"
#include "include/enemies/bowser.hpp"
#include "include/enemies/goomba.hpp"
#include "include/enemies/koopa_troopa.hpp"
#include "include/enemies/piranha_plant.hpp"
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
#include "include/collision/collision_handler.hpp"
#include <raylib.h>
#include <iostream>

int main() {
  SetTargetFPS(60);
  InitWindow(1200, 800, "Super Mario - Enemy Testing");
  App.Resource().Init();

  // Create character
  Character character(4.0f);
  character.SetCharacter(CharacterType::MARIO, {100, 400});

  // Create collision handler
  CollisionHandler collision(1200, 800);
  collision.AddCharacter(&character);

  // Create object manager
  ObjectManager &block = ObjectManager::GetInstance();
  block.Reset(4, &collision);

  // Create ground and blocks for testing
  block.AddBrickBlock({500, 300});
  
  // Create extended ground for enemy testing
  for (int i = 0; i < 80; ++i) {
    float x = 16 * 4 * i;
    block.AddStaticBlock({x, 600}, 'g'); // Ground level at 600
  }
  
  // Add some platforms for more interesting testing
  for (int i = 10; i < 20; ++i) {
    float x = 16 * 4 * i;
    block.AddStaticBlock({x, 400}, 'g'); // Platform level
  }
  
  // Add walls for collision testing
  for (int i = 0; i < 10; i++) {
    block.AddStaticBlock({0.0f, 600.0f - 16.0f * 4.0f * (float)i}, 'g'); // Left wall
    block.AddStaticBlock({1150.0f, 600.0f - 16.0f * 4.0f * (float)i}, 'g'); // Right wall
  }
  
  block.AddQuestionBlock({600, 300}, QuestionBlockItem::coin);
  block.AddFireFlower({800, 500});

  // Create projectile pool
  ProjectilePool pool(&collision);

  // ===== INITIALIZE ENEMY SYSTEM =====
  EnemyManager& enemyManager = EnemyManager::GetInstance();
  enemyManager.SetCollisionHandler(&collision);

  // Set character references for enemy AI
  Rectangle charRect = character.GetRectangle();
  Vector2 marioPos = {charRect.x, charRect.y};
  Vector2 luigiPos = {charRect.x, charRect.y};
  Vector2 activePos = {charRect.x, charRect.y};
  enemyManager.SetCharacterReferences(&marioPos, &luigiPos, &activePos);

  std::cout << "=== ENEMY TESTING ENVIRONMENT ===" << std::endl;
  std::cout << "Controls:" << std::endl;
  std::cout << "ARROW KEYS - Move Mario" << std::endl;
  std::cout << "UP - Jump" << std::endl;
  std::cout << "E - Evolve Mario" << std::endl;
  std::cout << "S - Star power" << std::endl;
  std::cout << "LEFT SHIFT - Shoot projectile" << std::endl;
  std::cout << "1 - Spawn Goomba" << std::endl;
  std::cout << "2 - Spawn Koopa" << std::endl;
  std::cout << "3 - Spawn Piranha Plant" << std::endl;
  std::cout << "4 - Spawn Bowser" << std::endl;
  std::cout << "C - Clear all enemies" << std::endl;
  std::cout << "R - Make Bowser attack" << std::endl;
  std::cout << "=====================================\n" << std::endl;

  //// Spawn some initial enemies for testing
  //enemyManager.SpawnEnemy(EnemyType::Goomba, {200.0f, 550.0f});
  //enemyManager.SpawnEnemy(EnemyType::Koopa, {400.0f, 550.0f});
  //enemyManager.SpawnEnemy(EnemyType::Piranha, {600.0f, 570.0f});

  // Set up initial movement for spawned enemies
  auto enemies = enemyManager.GetEnemies();
  for (Enemy* enemy : enemies) {
    if (enemy) {
      switch (enemy->GetType()) {
        case EnemyType::Goomba:
          enemy->SetVelocity({30.0f, 0.0f}); // Moving right
          enemy->SetFacing(false);
          break;
        case EnemyType::Koopa:
          enemy->SetVelocity({25.0f, 0.0f}); // Moving right
          enemy->SetFacing(false);
          break;
        case EnemyType::Piranha:
          // Piranha doesn't move horizontally
          enemy->SetVelocity({0.0f, 0.0f});
          break;
      }
    }
  }

  while (!WindowShouldClose()) {
    float deltaTime = GetFrameTime();

    BeginDrawing();
    ClearBackground(SKYBLUE);

    // Update character ground collision
    Rectangle rect = character.GetRectangle();
    if (rect.y + rect.height >= 600)
      character.StopY(600);

    // Update character position for enemy AI
    activePos = {rect.x, rect.y};
    marioPos = activePos;

    // Update all systems
    Enemy::SetFrameCount();
    enemyManager.UpdateAll(deltaTime);

    // Clean up dead enemies
    enemyManager.ClearDeadEnemies();

    // Check all collisions
    collision.CheckCollision();

    // Update character
    character.SetFrameCount();
    character.Update();

    // Update objects
    block.SetFrameCount();
    block.Update();

    // Update projectiles
    pool.SetFrameCount();
    pool.Update();

    // Draw everything
    character.Draw();
    block.Draw();
    pool.Draw();

    // Draw enemies with debug info
    for (Enemy* enemy : enemyManager.GetEnemies()) {
      if (enemy && enemy->IsAlive()) {
        // Draw the enemy sprite
        enemy->Draw();

        // Draw debug info overlay
        Rectangle enemyRect = enemy->GetRect();
        Vector2 velocity = enemy->GetVelocity();

        // Draw velocity for debugging
        DrawText(TextFormat("V:%.0f", velocity.x), (int)enemyRect.x,
                 (int)enemyRect.y - 20, 12, YELLOW);

        // Draw enemy type indicator
        const char* typeName = "";
        switch (enemy->GetType()) {
          case EnemyType::Goomba: typeName = "GOOMBA"; break;
          case EnemyType::Koopa: typeName = "KOOPA"; break;
          case EnemyType::Piranha: typeName = "PIRANHA"; break;
          case EnemyType::Bowser: typeName = "BOWSER"; break;
        }
        DrawText(typeName, (int)enemyRect.x, (int)enemyRect.y - 35, 10, WHITE);
        
        // Draw facing direction
        const char* direction = enemy->IsFacingLeft() ? "L" : "R";
        DrawText(direction, (int)enemyRect.x + (int)enemyRect.width - 15, (int)enemyRect.y - 20, 12, GREEN);
      }
    }

    // Draw UI info
    DrawText("Enemy Testing Environment", 10, 10, 20, WHITE);
    DrawText(TextFormat("Enemies: %d", (int)enemyManager.GetEnemyCount()), 10, 35, 16, WHITE);
    DrawText(TextFormat("Mario pos: %.0f, %.0f", activePos.x, activePos.y), 10, 55, 16, WHITE);
    
    // Draw instructions
    DrawText("1-4: Spawn enemies | C: Clear | R: Bowser attack", 10, GetScreenHeight() - 25, 14, LIGHTGRAY);

    // Handle input
    if (IsKeyDown(KEY_LEFT))
      character.Run(true);
    else if (IsKeyDown(KEY_RIGHT))
      character.Run(false);
    if (IsKeyDown(KEY_UP))
      character.Jump();
    if (IsKeyDown(KEY_E))
      character.Evolve();
    if (IsKeyDown(KEY_S))
      character.ToStarman();
    if (IsKeyDown(KEY_LEFT_SHIFT)) {
      Rectangle charRect2 = character.GetRectangle();
      pool.ShootElectricBall({charRect2.x, charRect2.y}, character.IsToLeft());
    }

    // Enemy spawn controls
    if (IsKeyPressed(KEY_ONE)) {
      // Spawn Goomba at random position
      float randomX = 200.0f + (float)(rand() % 600);
      enemyManager.SpawnEnemy(EnemyType::Goomba, {randomX, 550.0f});
      std::cout << "Spawned Goomba at " << randomX << std::endl;

      // Set movement for newly spawned enemy
      auto newEnemies = enemyManager.GetEnemies();
      if (!newEnemies.empty()) {
        Enemy* lastEnemy = newEnemies.back();
        if (lastEnemy && lastEnemy->GetType() == EnemyType::Goomba) {
          lastEnemy->SetVelocity({30.0f, 0.0f}); // Moving right
          lastEnemy->SetFacing(false);
        }
      }
    }
    
    if (IsKeyPressed(KEY_TWO)) {
      // Spawn Koopa at random position
      float randomX = 200.0f + (float)(rand() % 600);
      enemyManager.SpawnEnemy(EnemyType::Koopa, {randomX, 550.0f});
      std::cout << "Spawned Koopa at " << randomX << std::endl;

      // Set movement for newly spawned enemy
      auto newEnemies = enemyManager.GetEnemies();
      if (!newEnemies.empty()) {
        Enemy* lastEnemy = newEnemies.back();
        if (lastEnemy && lastEnemy->GetType() == EnemyType::Koopa) {
          lastEnemy->SetVelocity({25.0f, 0.0f}); // Moving right
          lastEnemy->SetFacing(false);
        }
      }
    }
    
    if (IsKeyPressed(KEY_THREE)) {
      // Spawn Piranha at random position
      float randomX = 200.0f + (float)(rand() % 600);
      enemyManager.SpawnEnemy(EnemyType::Piranha, {randomX, 570.0f});
      std::cout << "Spawned Piranha Plant at " << randomX << std::endl;
    }
    
    if (IsKeyPressed(KEY_FOUR)) {
      // Spawn Bowser at random position
      float randomX = 200.0f + (float)(rand() % 600);
      enemyManager.SpawnEnemy(EnemyType::Bowser, {randomX, 500.0f});
      std::cout << "Spawned Bowser at " << randomX << std::endl;

      // Set movement for newly spawned enemy
      auto newEnemies = enemyManager.GetEnemies();
      if (!newEnemies.empty()) {
        Enemy* lastEnemy = newEnemies.back();
        if (lastEnemy && lastEnemy->GetType() == EnemyType::Bowser) {
          lastEnemy->SetVelocity({15.0f, 0.0f}); // Moving right (slower)
          lastEnemy->SetFacing(false);
        }
      }
    }
    
    if (IsKeyPressed(KEY_C)) {
      enemyManager.ClearAllEnemies();
      std::cout << "Cleared all enemies" << std::endl;
    }

    // Test Bowser attack animation
    if (IsKeyPressed(KEY_R)) {
      for (Enemy* enemy : enemyManager.GetEnemies()) {
        if (enemy && enemy->GetType() == EnemyType::Bowser) {
          enemy->EnterAttackMode();
          std::cout << "Bowser entered attack mode" << std::endl;
        }
      }
    }

    EndDrawing();
  }

  CloseWindow();
  return 0;
}


//#include "include/core/application.hpp"
//#include <raylib.h>
//
//int main() {
//  App.Run();
//  return 0;
//}

