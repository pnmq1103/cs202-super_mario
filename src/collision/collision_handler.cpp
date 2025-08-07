#include <cmath>

#include "include/collision/collision_handler.hpp"
#include "include/managers/enemy_manager.hpp"

CollisionHandler::CollisionHandler(float width, float height)
    : cellSize_(16 * 4), projectile_list_(7, nullptr) {
  Reset(width, height);
}

void CollisionHandler::Reset(float width, float height) {
  std::cout << "reset collision handler\n";
  column_ = std::ceil(width / cellSize_);
  row_    = std::ceil(height / cellSize_);

  character_ = nullptr;
  for (int i = 0; i < 7; ++i)
    projectile_list_[i] = nullptr;
  object_list_.clear();
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
  x2      = std::ceil(a);
  y2      = std::ceil(b);
  if (x1 < 0)
    x1 = 0;
  if (y1 < 0)
    y1 = 0;
  if (x1 >= column_)
    x1 = column_ - 1;
  if (y1 >= row_)
    y1 = row_ - 1;

  if (x2 < 0)
    x2 = 0;
  if (y2 < 0)
    y2 = 0;
  if (x2 >= column_)
    x2 = column_ - 1;
  if (y2 >= row_)
    y2 = row_ - 1;

  return {y1, y2, x1, x2};
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
  if (character) {
    character_ = character;
  }
}

void CollisionHandler::AddProjectile(Projectile *projectile) {
  if (!projectile)
    return;
  for (int i = 0; i < projectile_list_.size(); ++i) {
    if (!projectile_list_[i]) {
      projectile_list_[i] = projectile;
      return;
    }
  }
}

void CollisionHandler::AddObject(GameObject *object) {
  if (!object)
    return;
  object_list_.push_back(object);
}

void CollisionHandler::AddEnemy(Enemy *enemy) {
  if (!enemy)
    return;
  enemy_list_.push_back(enemy);
}

void CollisionHandler::RemoveObject(int index) {
  object_list_[index] = nullptr;
  std::string s       = MakeID(2, index);
  if (previous_position_.find(s) != previous_position_.end()) {
    RemoveFromGrid(2, index, previous_position_[s]);
    previous_position_.erase(s);
  }
}

void CollisionHandler::RemoveEnemy(int index) {
  enemy_list_[index] = nullptr;
  std::string s      = MakeID(3, index);
  if (previous_position_.find(s) != previous_position_.end()) {
    RemoveFromGrid(3, index, previous_position_[s]);
    previous_position_.erase(s);
  }
}

void CollisionHandler::UpdatePosition() {

  for (int i = 0; i < projectile_list_.size(); ++i) {
    if (projectile_list_[i] && !projectile_list_[i]->IsDestroyed()) {
      UpdateToGrid(1, i, projectile_list_[i]->GetRectangle());
    }
  }
  for (int i = 0; i < object_list_.size(); ++i) {
    if (object_list_[i] && !object_list_[i]->IsDestroyed()) {
      UpdateToGrid(2, i, object_list_[i]->GetRectangle());
    }
  }
  for (int i = 0; i < enemy_list_.size(); ++i) {
    if (enemy_list_[i] && enemy_list_[i]->IsAlive()) {
      UpdateToGrid(3, i, enemy_list_[i]->GetRect());
    }
  }
}

void CollisionHandler::CheckCollision() {
  if (!character_)
    return;
  UpdatePosition();
  CheckCollisionCharacter();
  CheckCollisionProjectile();
  CheckCollisionEnemy();
  CheckCollisionMushroom();
}

void CollisionHandler::CheckCollisionCharacter() {

  std::vector<int> position = SearchLocation(character_->GetRectangle());

  for (int i = position[2]; i <= position[3]; ++i) {
    auto it = grid_[position[0]][i].begin();
    while (it != grid_[position[0]][i].end()) {
      int type = it->first, index = it->second;
      if (type == 1) {
        if (projectile_list_[index]->GetType() == enemy_fireball) {
          if (
            CheckCollisionRecs(
              character_->GetRectangle(),
              projectile_list_[index]->GetRectangle())
            && !character_->IsStarman())
            character_->Die();
        }
      } else if (type == 2) {
        if (
          object_list_[index]->GetType() == ObjectType::Block
          || object_list_[index]->GetType() == ObjectType::BrickBlock) {
          Rectangle rect      = character_->GetRectangle(),
                    rectangle = object_list_[index]->GetRectangle();
          Vector2 speed       = character_->GetSpeed();

          if (rect.y + speed.y <= rectangle.y + rectangle.height 
            && (rect.x +rect.width >= rectangle.x && rect.x <= rectangle.x + rectangle.width)) {

            character_->StopY();
            if (object_list_[index]->GetType() == ObjectType::BrickBlock) {
              if (character_->GetState() > 0)
                object_list_[index]->OnHit();
            } else
              object_list_[index]->OnHit();
          }
        } else {
          if (CheckCollisionRecs(
                character_->GetRectangle(),
                object_list_[index]->GetRectangle())) {
            object_list_[index]->OnHit();
            switch (object_list_[index]->GetType()) {
              case ObjectType::SuperStar:
                character_->ToStarman();
                break;
              case ObjectType::SuperMushroom:
                if (character_->GetState() == 0) {
                  bool isStarman = character_->IsStarman();
                  character_->Evolve();
                  if (isStarman)
                    character_->ToStarman();
                }
                break;

              case ObjectType::FireFlower:
                if (character_->GetState() <= 1) {
                  bool isStarman = character_->IsStarman();
                  character_->Evolve();
                  if (isStarman)
                    character_->ToStarman();
                }
                break;
              case ObjectType::Coin:
                break;
            }
          }
        }
      } else if (type == 3) {
        if (CheckCollisionRecs(
              character_->GetRectangle(), enemy_list_[index]->GetRect())) {
          if (character_->IsStarman())
            enemy_list_[index]->OnHitFromSide();
          else
            character_->Die();
        }
      }
      ++it;
    }

    it = grid_[position[1]][i].begin();
    while (it != grid_[position[1]][i].end()) {
      int type = it->first, index = it->second;
      if (type == 1) {
        if (projectile_list_[index]->GetType() == enemy_fireball) {
          if (
            CheckCollisionRecs(
              character_->GetRectangle(),
              projectile_list_[index]->GetRectangle())
            && !character_->IsStarman())
            character_->Die();
        }
      } else if (type == 2) {
        if (
          object_list_[index]->GetType() == ObjectType::Block
          || object_list_[index]->GetType() == ObjectType::BrickBlock) {
          Rectangle rect      = character_->GetRectangle(),
                    rectangle = object_list_[index]->GetRectangle();
          Vector2 speed       = character_->GetSpeed();

          if (
          rect.y + rect.height +speed.y >= rectangle.y 
          && (rect.x + rect.width >=rectangle.x && rect.x <= rectangle.x + rectangle.width)) {
            character_->StopY(rectangle.y);
          }
        } else {
          if (CheckCollisionRecs(
                character_->GetRectangle(),
                object_list_[index]->GetRectangle())) {
            object_list_[index]->OnHit();
            switch (object_list_[index]->GetType()) {
              case ObjectType::SuperStar:
                character_->ToStarman();
                break;
              case ObjectType::SuperMushroom:
                if (character_->GetState() == 0) {
                  bool isStarman = character_->IsStarman();
                  character_->Evolve();
                  if (isStarman)
                    character_->ToStarman();
                }
                break;

              case ObjectType::FireFlower:
                if (character_->GetState() <= 1) {
                  bool isStarman = character_->IsStarman();
                  character_->Evolve();
                  if (isStarman)
                    character_->ToStarman();
                }
                break;
              case ObjectType::Coin:
                break;
            }
          }
        }
      } else if (type == 3) {
        if (CheckCollisionRecs(
              character_->GetRectangle(), enemy_list_[index]->GetRect())) {
          if (character_->IsStarman()) {
            enemy_list_[index]->OnHitFromSide();
          } else {
            enemy_list_[index]->OnHitFromAbove();
            if (enemy_list_[index]->GetType() == EnemyType::Piranha)
              character_->Die();
            else
              character_->Bounce();
          }
        }
      }

      ++it;
    }
  }

  for (int i = position[0]; i <= position[1]; ++i) {
    auto it = grid_[i][position[2]].begin();
    while (it != grid_[i][position[2]].end()) {
      int type = it->first, index = it->second;
      if (type == 1) {
        if (projectile_list_[index]->GetType() == enemy_fireball) {
          if (
            CheckCollisionRecs(
              character_->GetRectangle(),
              projectile_list_[index]->GetRectangle())
            && !character_->IsStarman())
            character_->Die();
        }
      } else if (type == 2) {
        if (
          object_list_[index]->GetType() == ObjectType::Block
          || object_list_[index]->GetType() == ObjectType::BrickBlock) {
          Rectangle rect      = character_->GetRectangle(),
                    rectangle = object_list_[index]->GetRectangle();
          Vector2 speed       = character_->GetSpeed();

          if (
            rect.x + speed.x <= rectangle.x + rectangle.width
            && rect.x > rectangle.x + rectangle.width
            && !(
              rect.y >= rectangle.y + rectangle.height
              || rect.y + rect.height <= rectangle.y)) {
            character_->StopX();
          }
        } else {
          if (CheckCollisionRecs(
                character_->GetRectangle(),
                object_list_[index]->GetRectangle())) {
            object_list_[index]->OnHit();
            switch (object_list_[index]->GetType()) {
              case ObjectType::SuperStar:
                character_->ToStarman();
                break;
              case ObjectType::SuperMushroom:
                if (character_->GetState() == 0) {
                  bool isStarman = character_->IsStarman();
                  character_->Evolve();
                  if (isStarman)
                    character_->ToStarman();
                }
                break;

              case ObjectType::FireFlower:
                if (character_->GetState() <= 1) {
                  bool isStarman = character_->IsStarman();
                  character_->Evolve();
                  if (isStarman)
                    character_->ToStarman();
                }
                break;
              case ObjectType::Coin:
                break;
            }
          }
        }

      } else if (type == 3) {
        if (CheckCollisionRecs(
              character_->GetRectangle(), enemy_list_[index]->GetRect())) {
          if (character_->IsStarman())
            enemy_list_[index]->OnHitFromSide();
          // else
          // character_->Die();
        }
      }
      ++it;
    }

    it = grid_[i][position[3]].begin();
    while (it != grid_[i][position[3]].end()) {
      int type = it->first, index = it->second;
      if (type == 1) {
        if (projectile_list_[index]->GetType() == enemy_fireball) {
          if (
            CheckCollisionRecs(
              character_->GetRectangle(),
              projectile_list_[index]->GetRectangle())
            && !character_->IsStarman())
            character_->Die();
        }
      } else if (type == 2) {
        if (
          object_list_[index]->GetType() == ObjectType::Block
          || object_list_[index]->GetType() == ObjectType::BrickBlock) {
          Rectangle rect      = character_->GetRectangle(),
                    rectangle = object_list_[index]->GetRectangle();
          Vector2 speed       = character_->GetSpeed();

          if (
            rect.x + rect.width + speed.x >= rectangle.x
            && rect.x + rect.width < rectangle.x
            && !(
              rect.y >= rectangle.y + rectangle.height
              || rect.y + rect.height <= rectangle.y)) {
            character_->StopX();
          }
        } else {
          if (CheckCollisionRecs(
                character_->GetRectangle(),
                object_list_[index]->GetRectangle())) {
            object_list_[index]->OnHit();
            switch (object_list_[index]->GetType()) {
              case ObjectType::SuperStar:
                character_->ToStarman();
                break;
              case ObjectType::SuperMushroom:
                if (character_->GetState() == 0) {
                  bool isStarman = character_->IsStarman();
                  character_->Evolve();
                  if (isStarman)
                    character_->ToStarman();
                }
                break;

              case ObjectType::FireFlower:
                if (character_->GetState() <= 1) {
                  bool isStarman = character_->IsStarman();
                  character_->Evolve();
                  if (isStarman)
                    character_->ToStarman();
                }
                break;
              case ObjectType::Coin:
                break;
            }
          }
        }

      } else if (type == 3) {
        if (CheckCollisionRecs(
              character_->GetRectangle(), enemy_list_[index]->GetRect())) {
          if (character_->IsStarman())
            enemy_list_[index]->OnHitFromSide();
          // else
          // character_->Die();
        }
      }
      ++it;
    }
  }
}

void CollisionHandler::CheckCollisionProjectile() {
  for (int i = 0; i < projectile_list_.size(); ++i) {
    if (projectile_list_[i] && !projectile_list_[i]->IsDestroyed()) {
      Rectangle rect            = projectile_list_[i]->GetRectangle();
      std::vector<int> position = SearchLocation(rect);
      for (int j = position[0]; j <= position[1]; ++j) {
        auto it = grid_[j][position[2]].begin();
        while (it != grid_[j][position[2]].end()) {
          int type = it->first, index = it->second;

          Rectangle rectangle = object_list_[index]->GetRectangle();
          Vector2 speed       = projectile_list_[i]->GetSpeed();

          if (
            rect.x + speed.x <= rectangle.x + rectangle.width
            && rect.x > rectangle.x + rectangle.width
            && !(
              rect.y >= rectangle.y + rectangle.height
              || rect.y + rect.height <= rectangle.y)) {
            if (projectile_list_[i]->GetType() == mario_fireball)
              projectile_list_[i]->ReverseDirection();
            else
              projectile_list_[i]->Destroy();
          }

          ++it;
        }

        it = grid_[j][position[3]].begin();
        while (it != grid_[j][position[3]].end()) {
          int type = it->first, index = it->second;

          Rectangle rectangle = object_list_[index]->GetRectangle();
          Vector2 speed       = projectile_list_[i]->GetSpeed();

          if (type == 2) {
            if (
              rect.x + rect.width + speed.x >= rectangle.x
              && rect.x + rect.width < rectangle.x
              && !(
                rect.y >= rectangle.y + rectangle.height
                || rect.y + rect.height <= rectangle.y)) {
              if (projectile_list_[i]->GetType() == mario_fireball)
                projectile_list_[i]->ReverseDirection();
              else
                projectile_list_[i]->Destroy();
            }
          }
          ++it;
        }
      }

      if (projectile_list_[i]->GetType() == mario_fireball) {
        bool stop_upper = false, stop_lower = false;

        for (int j = position[2]; j <= position[3]; ++j) {
          auto it = grid_[position[0]][j].begin();
          while (it != grid_[position[0]][j].end()) {
            int type = it->first, index = it->second;
            if (type == 2) {
              Rectangle rectangle = object_list_[index]->GetRectangle();
              Vector2 speed       = projectile_list_[i]->GetSpeed();
              if (rect.y + speed.y <= rectangle.y + rectangle.height &&
                  rect.y + rect.height>rectangle.y + rectangle.height &&
                 (rect.x +rect.width >= rectangle.x && rect.x <= rectangle.x + rectangle.width))
                stop_upper = true;
            }
            ++it;
          }

          it = grid_[position[1]][j].begin();
          while (it != grid_[position[1]][j].end()) {
            int type = it->first, index = it->second;
            if (type == 2) {
              Rectangle rectangle = object_list_[index]->GetRectangle();
              Vector2 speed       = projectile_list_[i]->GetSpeed();
              if (rect.y + rect.height +speed.y >= rectangle.y && (rect.x + rect.width >=rectangle.x
          && rect.x <= rectangle.x + rectangle.width))
                stop_lower = true;
            }
            ++it;
          }
        }

        projectile_list_[i]->StopY(stop_upper, stop_lower);
      }
    }
  }
}

void CollisionHandler::CheckCollisionEnemy() {

  for (int i = 0; i < enemy_list_.size(); ++i) {
    if (enemy_list_[i] && enemy_list_[i]->IsAlive()) {
      Rectangle rect            = enemy_list_[i]->GetRect();
      std::vector<int> position = SearchLocation(rect);
      Vector2 speed             = enemy_list_[i]->GetVelocity();

      // ===== BOUNDARY CHECKING =====
      float mapWidth  = column_ * cellSize_;
      float mapHeight = row_ * cellSize_;

      // Check if enemy is outside map boundaries
      bool outOfBounds = false;

      // Check horizontal boundaries (left/right)
      if (rect.x + rect.width + speed.x < 0 || rect.x + speed.x > mapWidth) {
        if (enemy_list_[i]->GetType() == EnemyType::Bowser) {
          enemy_list_[i]->ReverseDirection();
        } else {
          // Goomba/Koopa/Piranha: remove when out of map horizontally
          outOfBounds = true;
        }
      }

      // Check vertical boundary (fall off bottom)
      if (rect.y > mapHeight) {
        outOfBounds = true;
      }

      // Remove enemy if out of bounds
      if (outOfBounds) {
        EnemyManager::GetInstance().RemoveEnemy(i);
        return;
      }
      // ===== END BOUNDARY CHECKING =====

      // Existing collision logic with blocks and projectiles (Same)

      for (int j = position[2]; j <= position[3]; ++j) {

        auto it = grid_[position[1]][j].begin();

        while (it != grid_[position[1]][j].end()) {
          int type = it->first, index = it->second;
          if (type == 2) {
            Rectangle rectangle = object_list_[index]->GetRectangle();

            if (
          rect.y + rect.height >= rectangle.y && (rect.x + rect.width
      >=rectangle.x
          && rect.x <= rectangle.x + rectangle.width)) {
              enemy_list_[i]->StopY(rectangle.y);
            }
          }
          ++it;
        }
      }

      for (int j = position[0]; j <= position[1]; ++j) {

        auto it = grid_[j][position[2]].begin();

        while (it != grid_[j][position[2]].end()) {
          int type = it->first, index = it->second;
          if (type == 1) {
            if (
              CheckCollisionRecs(rect, projectile_list_[index]->GetRectangle())
              && projectile_list_[index]->GetType() != enemy_fireball) {
              enemy_list_[i]->OnHitFromSide();
              projectile_list_[index]->Destroy();
            }
          } else if (type == 2) {
            Rectangle rect      = enemy_list_[i]->GetRectangle(),
                      rectangle = object_list_[index]->GetRectangle();
            Vector2 speed       = enemy_list_[i]->GetVelocity();
            if (
              rect.x + speed.x <= rectangle.x + rectangle.width
              && rect.x > rectangle.x + rectangle.width
              && !(
                rect.y >= rectangle.y + rectangle.height
                || rect.y + rect.height <= rectangle.y)) {
              enemy_list_[i]->ReverseDirection();
            }
          }
          ++it;
        }

        it = grid_[j][position[3]].begin();

        while (it != grid_[j][position[3]].end()) {
          int type = it->first, index = it->second;
          if (type == 1) {
            if (
              CheckCollisionRecs(rect, projectile_list_[index]->GetRectangle())
              && projectile_list_[index]->GetType() != enemy_fireball) {
              enemy_list_[i]->OnHitFromSide();
              projectile_list_[index]->Destroy();
            }
          } else if (type == 2) {
            Rectangle rect      = enemy_list_[i]->GetRectangle(),
                      rectangle = object_list_[index]->GetRectangle();
            Vector2 speed       = enemy_list_[i]->GetVelocity();
            if (
              rect.x + rect.width + speed.x >= rectangle.x
              && rect.x + rect.width < rectangle.x
              && !(
                rect.y >= rectangle.y + rectangle.height
                || rect.y + rect.height <= rectangle.y)) {
              enemy_list_[i]->ReverseDirection();
            }
          }
          ++it;
        }
      }
    }
  }
}

void CollisionHandler::CheckCollisionMushroom() {
  for (int j = 0; j < object_list_.size(); ++j) {
    if (
      object_list_[j] && !object_list_[j]->IsDestroyed()
      && object_list_[j]->GetType() == ObjectType::SuperMushroom) {
      std::vector<int> position
        = SearchLocation(object_list_[j]->GetRectangle());
      for (int i = position[2]; i <= position[3]; ++i) {
        auto it = grid_[position[1]][i].begin();
        while (it != grid_[position[1]][i].end()) {
          int type = it->first, index = it->second;
          if (j == index) {
            ++it;
            continue;
          }

          if (type == 2) {
            if (object_list_[index]->GetType() != ObjectType::SuperMushroom) {
              Rectangle rect      = object_list_[j]->GetRectangle(),
                        rectangle = object_list_[index]->GetRectangle();
              Vector2 speed       = object_list_[j]->GetSpeed();

              if (
          rect.y + rect.height  >= rectangle.y
          && (rect.x + rect.width >=rectangle.x && rect.x <= rectangle.x +
          rectangle.width)) {
                object_list_[j]->StopY(rectangle.y);
              }
            }
          }

          ++it;
        }
      }

      for (int i = position[0]; i <= position[1]; ++i) {
        auto it = grid_[i][position[2]].begin();
        while (it != grid_[i][position[2]].end()) {
          int type = it->first, index = it->second;
          if (j == index) {
            ++it;
            continue;
          }
          if (type == 2) {
            if (object_list_[index]->GetType() != ObjectType::SuperMushroom) {
              Rectangle rect      = object_list_[j]->GetRectangle(),
                        rectangle = object_list_[index]->GetRectangle();
              Vector2 speed       = object_list_[j]->GetSpeed();

              if (
                rect.x + speed.x <= rectangle.x + rectangle.width
                && rect.x > rectangle.x + rectangle.width
                && !(
                  rect.y >= rectangle.y + rectangle.height
                  || rect.y + rect.height <= rectangle.y)) {
                object_list_[j]->ReverseDirection();
              }
            }
          }
          ++it;
        }

        it = grid_[i][position[3]].begin();
        while (it != grid_[i][position[3]].end()) {
          int type = it->first, index = it->second;
          if (j == index) {
            ++it;
            continue;
          }
          if (type == 2) {
            if (object_list_[index]->GetType() != ObjectType::SuperMushroom) {
              Rectangle rect      = object_list_[j]->GetRectangle(),
                        rectangle = object_list_[index]->GetRectangle();
              Vector2 speed       = object_list_[j]->GetSpeed();
              if (
                rect.x + rect.width + speed.x >= rectangle.x
                && rect.x + rect.width < rectangle.x
                && !(
                  rect.y >= rectangle.y + rectangle.height
                  || rect.y + rect.height <= rectangle.y)) {
                object_list_[j]->ReverseDirection();
              }
            }
          }
          ++it;
        }
      }
    }
  }
}