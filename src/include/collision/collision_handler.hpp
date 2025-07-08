#pragma once
#include "include/blocks/question_block.hpp"
#include "include/blocks/static_block.hpp"
#include "include/characters/character.hpp"
#include "include/characters/fireball.hpp"
#include "include/enemies/bowser.hpp"
#include "include/enemies/goomba.hpp"
#include "include/enemies/koopa_troopa.hpp"
#include "include/enemies/piranha_plant.hpp"
#include <iostream>
#include <list>
#include <unordered_map>
#include <utility>
#include <vector>

class CollisionHandler {
private:
  Character *character_;
  std::vector<Projectile *> projectile_list_;
  std::vector<Block *> block_list_;
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

public:
  CollisionHandler(float width, float height);
  ~CollisionHandler() = default;
  void Reset(float width, float height);
  void AddCharacter(Character *character);
  void AddProjectile(Projectile *projectile);
  void AddBlock(Block *block);
  void AddEnemy(Enemy *enemy);
  void CheckCollision();
};