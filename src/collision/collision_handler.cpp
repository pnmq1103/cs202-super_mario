#include "include/collision/collision_handler.hpp"
#include <cmath>

CollisionHandler::CollisionHandler(float width, float height)
    : cellSize_(16 * 4), projectile_list_(7, nullptr) {
  Reset(width, height);
}

void CollisionHandler::Reset(float width, float height) {
  column_ = std::ceil(width / cellSize_);
  row_    = std::ceil(height / cellSize_);

  character_ = nullptr;
  for (int i = 0; i < 7; ++i)
    projectile_list_[i] = nullptr;
  block_list_.clear();
  enemy_list_.clear();

  grid_.resize(row_);
  for (int i = 0; i < grid_.size(); ++i) {
    grid_[i].resize(column_);
    for (int j = 0; j < grid_[i].size(); ++j) {
      grid_[i][j].clear();
    }
  }
  previous_position_.clear();
}

std::string CollisionHandler::MakeID(int type, int index) {
  return std::to_string(type) + "," + std::to_string(index);
}

std::vector<int> CollisionHandler::SearchLocation(Rectangle rectangle) {
  int x1  = std::floor(rectangle.x / cellSize_),
      y1  = std::floor(rectangle.y / cellSize_), x2, y2;
  float a = (rectangle.x + rectangle.width) / cellSize_,
        b = (rectangle.y + rectangle.height) / cellSize_;
  if (a == std::floor(a)) {
    x2 = a - 1;
  } else
    x2 = std::floor(a);
  if (b == std::floor(b)) {
    y2 = b - 1;
  } else
    y2 = std::floor(b);

  if (x1 < 0)
    x1 = 0;
  if (y1 < 0)
    y1 = 0;
  if (x2 >= row_)
    x2 = row_ - 1;
  if (y2 >= column_)
    y2 = column_ - 1;

  return {x1, x2, y1, y2};
}

void
CollisionHandler::AddToGrid(int type, int index, std::vector<int> position) {
  if (position.size() != 4)
    throw std::runtime_error("invalid position vector\n");
  for (int i = position[0]; i <= position[1]; ++i) {
    for (int j = position[2]; j <= position[3]; ++j) {
      grid_[i][j].push_back({type, index});
    }
  }
}
void CollisionHandler::RemoveFromGrid(
  int type, int index, std::vector<int> position) {
  if (position.size() != 4)
    throw std::runtime_error("invalid position vector\n");
  for (int i = position[0]; i <= position[1]; ++i) {
    for (int j = position[2]; j <= position[3]; ++j) {
      auto it = grid_[i][j].begin();
      while (it != grid_[i][j].end()) {
        if (it->first == type && it->second == index) {
          it = grid_[i][j].erase(it);
        } else
          ++it;
      }
    }
  }
}

void CollisionHandler::UpdateToGrid(int type, int index, Rectangle rectangle) {
  std::string s             = MakeID(type, index);
  std::vector<int> position = SearchLocation(rectangle);
  bool different_position   = true;

  if (previous_position_.find(s) == previous_position_.end()) {
    previous_position_.insert({s, position});
  } else {
    if (position == previous_position_[s]) {
      different_position = false;
    } else {
      RemoveFromGrid(type, index, previous_position_[s]);
      previous_position_[s] = position;
    }
  }
  if (different_position)
    AddToGrid(type, index, position);
}

void CollisionHandler::AddCharacter(Character *character) {
  if (!character_ && character) {
    character_ = character;
    UpdateToGrid(1, 0, character_->GetRectangle());
  }
}

void CollisionHandler::AddProjectile(Projectile *projectile) {
  if (!projectile)
    return;
  for (int i = 0; i < projectile_list_.size(); ++i) {
    if (!projectile_list_[i]) {
      projectile_list_[i] = projectile;
      UpdateToGrid(2, i, projectile->GetRectangle());
      return;
    }
  }
}

void CollisionHandler::AddBlock(Block *block) {
  if (!block)
    return;
  block_list_.push_back(block);
  UpdateToGrid(3, block_list_.size() - 1, block->GetRect());
}

void CollisionHandler::AddEnemy(Enemy *enemy) {
  if (!enemy)
    return;
  enemy_list_.push_back(enemy);
  UpdateToGrid(4, enemy_list_.size() - 1, enemy->GetRect());
}

void CollisionHandler::UpdatePosition() {
  if (character_) {
    UpdateToGrid(1, 0, character_->GetRectangle());
  }
  for (int i = 0; i < projectile_list_.size(); ++i) {
    if (projectile_list_[i] && !projectile_list_[i]->IsDestroyed()) {
      UpdateToGrid(2, i, projectile_list_[i]->GetRectangle());
    }
  }
  for (int i = 0; i < block_list_.size(); ++i) {
    if (block_list_[i]) {}
  }
  for (int i = 0; i < enemy_list_.size(); ++i) {
    if (enemy_list_[i] && enemy_list_[i]->IsAlive()) {
      UpdateToGrid(4, i, enemy_list_[i]->GetRect());
    }
  }
}