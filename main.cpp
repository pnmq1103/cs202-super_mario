// #include "include/characters/character.hpp"
// #include "include/characters/projectile_pool.hpp"
// #include "include/collision/collision_handler.hpp"
// #include "include/core/application.hpp"
// #include "include/enemies/bowser.hpp"
// #include "include/enemies/goomba.hpp"
// #include "include/enemies/koopa_troopa.hpp"
// #include "include/enemies/piranha_plant.hpp"
// #include "include/managers/enemy_manager.hpp"
// #include "include/objects/brick_block.hpp"
// #include "include/objects/coin.hpp"
// #include "include/objects/fire_flower.hpp"
// #include "include/objects/object_manager.hpp"
// #include "include/objects/pipe_block.hpp"
// #include "include/objects/question_block.hpp"
// #include "include/objects/static_block.hpp"
// #include "include/objects/super_mushroom.hpp"
// #include "include/objects/super_star.hpp"
// #include <iostream>
// #include <raylib.h>

// int main() {
//   SetTargetFPS(60);
//   InitWindow(1200, 800, "Super Mario - Enemy Testing");
//   App.Resource().Init();
//
//   // Create character
//   Character character(4.0f);
//   character.SetCharacter(CharacterType::MARIO, {1100, 100});
//
//   // Create collision handler
//   CollisionHandler collision(1200, 800);
//   collision.AddCharacter(&character);
//
//   // Create object manager
//   ObjectManager &block = ObjectManager::GetInstance();
//   block.Reset(4, &collision);
//
//   // Create extended ground for enemy testing
//   for (int i = 1; i < 80; ++i) {
//     float x = 16 * 4 * i;
//     block.AddStaticBlockByTheme({x, 600}, 'g'); // Ground level at 600
//   }
//
//   // Add some platforms for more interesting testing
//   for (int i = 10; i < 20; ++i) {
//     float x = 16 * 4 * i;
//     block.AddStaticBlockByTheme({x, 400}, 'g'); // Platform level
//   }
//
//   // Add walls for collision testing
//   for (int i = 0; i < 10; i++) {
//     block.AddStaticBlockByTheme(
//       {0.0f, 600.0f - 16.0f * 4.0f * (float)i}, 'g'); // Left wall
//     block.AddStaticBlockByTheme(
//       {1150.0f, 600.0f - 16.0f * 4.0f * (float)i}, 'g'); // Right wall
//   }
//
//   block.AddQuestionBlock({900, 200}, QuestionBlockItem::coin);
//   block.AddFireFlower({1000, 200});
//
//   // Create projectile pool
//   ProjectilePool pool(&collision);
//
//   // ===== INITIALIZE ENEMY SYSTEM =====
//   EnemyManager &enemyManager = EnemyManager::GetInstance();
//   enemyManager.SetCollisionHandler(&collision);
//
//   // Set character references for enemy AI
//   Rectangle charRect = character.GetRectangle();
//   Vector2 marioPos   = {charRect.x, charRect.y};
//   Vector2 luigiPos   = {charRect.x, charRect.y};
//   Vector2 activePos  = {charRect.x, charRect.y};
//   enemyManager.SetCharacterReferences(&marioPos, &luigiPos, &activePos);
//
//   //// Spawn some initial enemies for testing
//   enemyManager.SpawnEnemy(EnemyType::Goomba, {200.0f, 200.0f});
//   enemyManager.SpawnEnemy(EnemyType::Koopa, {400.0f, 550.0f});
//   //  Set up initial movement for spawned enemies
//   auto enemies = enemyManager.GetEnemies();
//   for (Enemy *enemy : enemies) {
//     if (enemy) {
//       switch (enemy->GetType()) {
//         case EnemyType::Goomba:
//           enemy->SetVelocity({5.0f, 0.0f}); // Moving right
//           enemy->SetFacing(false);
//           break;
//         case EnemyType::Koopa:
//           enemy->SetVelocity({10.0f, 0.0f}); // Moving right
//           enemy->SetFacing(false);
//           break;
//         case EnemyType::Piranha:
//           // Piranha doesn't move horizontally
//           enemy->SetVelocity({0.0f, 0.0f});
//           break;
//       }
//     }
//   }
//
//   while (!WindowShouldClose()) {
//     float deltaTime = GetFrameTime();
//
//     BeginDrawing();
//     ClearBackground(SKYBLUE);
//
//     // Update character ground collision
//     Rectangle rect = character.GetRectangle();
//
//     // Update character position for enemy AI
//     activePos = {rect.x, rect.y};
//     marioPos  = activePos;
//
//     // Clean up dead enemies
//     // enemyManager.ClearDeadEnemies();
//
//     // Check all collisions
//
//     collision.CheckCollision();
//     // Update all systems
//     Enemy::SetFrameCount();
//     enemyManager.UpdateAll(deltaTime);
//     enemyManager.Draw();
//
//     // Update character
//     character.SetFrameCount();
//     character.Update();
//
//     // Update objects
//     block.SetFrameCount();
//     block.Update();
//
//     // Update projectiles
//     pool.SetFrameCount();
//     pool.Update();
//
//     // Draw everything
//     character.Draw();
//     block.Draw();
//     pool.Draw();
//
//     // Handle input
//     if (IsKeyDown(KEY_LEFT))
//       character.Run(true);
//     else if (IsKeyDown(KEY_RIGHT))
//       character.Run(false);
//     if (IsKeyDown(KEY_UP))
//       character.Jump();
//     if (IsKeyDown(KEY_E))
//       character.Evolve();
//     if (IsKeyDown(KEY_S))
//       character.ToStarman();
//     if (IsKeyDown(KEY_LEFT_SHIFT)) {
//       Rectangle charRect2 = character.GetRectangle();
//       pool.ShootMarioFireball({charRect2.x, charRect2.y},
//       character.IsToLeft());
//     }
//
//     // Enemy spawn controls
//     /*if (IsKeyPressed(KEY_ONE)) {
//       // Spawn Goomba at random position
//       float randomX = 200.0f + (float)(rand() % 600);
//       enemyManager.SpawnEnemy(EnemyType::Goomba, {randomX, 550.0f});
//       std::cout << "Spawned Goomba at " << randomX << std::endl;
//
//       // Set movement for newly spawned enemy
//       auto newEnemies = enemyManager.GetEnemies();
//       if (!newEnemies.empty()) {
//         Enemy *lastEnemy = newEnemies.back();
//         if (lastEnemy && lastEnemy->GetType() == EnemyType::Goomba) {
//           lastEnemy->SetVelocity({30.0f, 0.0f}); // Moving right
//           lastEnemy->SetFacing(false);
//         }
//       }
//     }
//
//     if (IsKeyPressed(KEY_TWO)) {
//       // Spawn Koopa at random position
//       float randomX = 200.0f + (float)(rand() % 600);
//       enemyManager.SpawnEnemy(EnemyType::Koopa, {randomX, 550.0f});
//       std::cout << "Spawned Koopa at " << randomX << std::endl;
//
//       // Set movement for newly spawned enemy
//       auto newEnemies = enemyManager.GetEnemies();
//       if (!newEnemies.empty()) {
//         Enemy *lastEnemy = newEnemies.back();
//         if (lastEnemy && lastEnemy->GetType() == EnemyType::Koopa) {
//           lastEnemy->SetVelocity({25.0f, 0.0f}); // Moving right
//           lastEnemy->SetFacing(false);
//         }
//       }
//     }
//
//     if (IsKeyPressed(KEY_THREE)) {
//       // Spawn Piranha at random position
//       float randomX = 200.0f + (float)(rand() % 600);
//       enemyManager.SpawnEnemy(EnemyType::Piranha, {randomX, 570.0f});
//       std::cout << "Spawned Piranha Plant at " << randomX << std::endl;
//     }
//
//     if (IsKeyPressed(KEY_FOUR)) {
//       // Spawn Bowser at random position
//       float randomX = 200.0f + (float)(rand() % 600);
//       enemyManager.SpawnEnemy(EnemyType::Bowser, {randomX, 500.0f});
//       std::cout << "Spawned Bowser at " << randomX << std::endl;
//
//       // Set movement for newly spawned enemy
//       auto newEnemies = enemyManager.GetEnemies();
//       if (!newEnemies.empty()) {
//         Enemy *lastEnemy = newEnemies.back();
//         if (lastEnemy && lastEnemy->GetType() == EnemyType::Bowser) {
//           lastEnemy->SetVelocity({15.0f, 0.0f}); // Moving right (slower)
//           lastEnemy->SetFacing(false);
//         }
//       }
//     }
//
//     if (IsKeyPressed(KEY_C)) {
//       enemyManager.ClearAllEnemies();
//       std::cout << "Cleared all enemies" << std::endl;
//     }
//
//     // Test Bowser attack animation
//     if (IsKeyPressed(KEY_R)) {
//       for (Enemy *enemy : enemyManager.GetEnemies()) {
//         if (enemy && enemy->GetType() == EnemyType::Bowser) {
//           enemy->EnterAttackMode();
//           std::cout << "Bowser entered attack mode" << std::endl;
//         }
//       }
//     }*/
//
//     EndDrawing();
//   }
//
//   CloseWindow();
//   return 0;
// }

// #include "include/characters/character.hpp"
// #include "include/characters/projectile_pool.hpp"
// #include "include/collision/collision_handler.hpp"
// #include "include/core/application.hpp"
// #include "include/enemies/bowser.hpp"
// #include "include/enemies/goomba.hpp"
// #include "include/enemies/koopa_troopa.hpp"
// #include "include/enemies/piranha_plant.hpp"
// #include "include/managers/enemy_manager.hpp"
// #include "include/objects/brick_block.hpp"
// #include "include/objects/coin.hpp"
// #include "include/objects/fire_flower.hpp"
// #include "include/objects/object_manager.hpp"
// #include "include/objects/pipe_block.hpp"
// #include "include/objects/question_block.hpp"
// #include "include/objects/static_block.hpp"
// #include "include/objects/super_mushroom.hpp"
// #include "include/objects/super_star.hpp"
// #include <iostream>
// #include <raylib.h>
// int main() {
//   SetTargetFPS(60);
//   InitWindow(1200, 800, "Super Mario - Enemy Testing");
//   App.Resource().Init();
//   CollisionHandler collision(1200, 800);
//   ObjectManager &block = ObjectManager::GetInstance();
//   block.Reset(4, &collision);
//   block.AddQuestionBlock({100, 100}, QuestionBlockItem::super_mushroom);
//   for (int i = 1; i < 10; ++i) {
//     block.AddStaticBlockByTheme({(float)64 * i, 500}, 'g');
//   }
//   block.AddStaticBlockByTheme({10 * 64, 500 - 64}, 'g');
//   block.AddStaticBlockByTheme({0, 500 - 64}, 'g');
//   while (!WindowShouldClose()) {
//     BeginDrawing();
//     ClearBackground(BLACK);
//     collision.CheckCollision();
//     block.SetFrameCount();
//     block.Update();
//     block.Draw();
//     if (IsKeyDown(KEY_A))
//       block.OnHit(0);
//     EndDrawing();
//   }
//   return 0;
// }

#include "include/core/application.hpp"
#include <raylib.h>
int main() {
  App.Run();
  return 0;
}