#pragma once
#include "include/enemies/enemy.hpp"
#include <vector>
#include <memory>

class EnemyManager {
private:
  std::vector<Enemy*> enemies;

public:
  ~EnemyManager();

  void AddEnemy(Enemy* enemy);
  void UpdateAll(float dt);
  void RenderAll(Texture &tex, const std::unordered_map<int, Rectangle> &spriteRects);
  void ClearDeadEnemies(); 

  std::vector<Enemy*>& GetEnemies(); 
};
