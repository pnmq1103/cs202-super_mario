#pragma once
#include <iostream>
#include <list>
#include <unordered_map>
#include <utility>
#include <vector>

#include "include/characters/character.hpp"
#include "include/characters/fireball.hpp"
#include "include/enemies/enemy.hpp"
#include "include/objects/game_object.hpp"

class CollisionHandler {
private:
  Character *character_;
  std::vector<Projectile *> projectile_list_;
  std::vector<GameObject *> object_list_;
  std::vector<Enemy *> enemy_list_;

  std::vector<std::vector<std::list<std::pair<int, int>>>> grid_;
  std::unordered_map<std::string, std::vector<int>> previous_position_;

  int row_, column_;
  const float cellSize_;

  std::string MakeID(int type, int index);
  std::vector<int> SearchLocation(Rectangle rectangle);
  void AddToGrid(int type, int index, std::vector<int> position);
  void RemoveFromGrid(int type, int index, std::vector<int> position);
  void UpdateToGrid(int type, int index, Rectangle rectangle);
  void UpdatePosition();
  void CheckCollisionCharacter();
  void CheckCollisionProjectile();
  void CheckCollisionEnemy();

public:
  CollisionHandler(float width, float height);
  ~CollisionHandler() = default;
  void Reset(float width, float height);

  void AddCharacter(Character *character);
  void AddProjectile(Projectile *projectile);
  void AddObject(GameObject *object);
  void AddEnemy(Enemy *enemy);

  void RemoveObject(int index);
  void RemoveEnemy(int index);
  void CheckCollision();
};