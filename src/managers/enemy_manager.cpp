#include "include/managers/enemy_manager.hpp"

EnemyManager::~EnemyManager() {
  for (Enemy* e : enemies) {
    delete e;
  }
}

void EnemyManager::AddEnemy(Enemy* enemy) {
  enemies.push_back(enemy);
}

void EnemyManager::UpdateAll(float dt) {
  for (Enemy* e : enemies) {
    if (e && e->IsAlive())
      e->Update(dt);
  }
}

void EnemyManager::RenderAll(Texture &tex, const std::unordered_map<int, Rectangle> &spriteRects) {
  for (Enemy* e : enemies) {
    if (e && e->IsAlive())
      e->Render(tex, spriteRects);
  }
}

void EnemyManager::ClearDeadEnemies() {
  for (Enemy*& e : enemies) {
    if (e && !e->IsAlive()) {
      e = nullptr;
    }
  }
}

std::vector<Enemy*>& EnemyManager::GetEnemies() {
  return enemies;
}
