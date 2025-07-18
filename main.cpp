/*#include "include/core/application.hpp"
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
#include <cmath>
#include <iostream>
#include <list>
#include <raylib.h>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
class Collision {
private:
  const float cellSize_;
  float row_, column_;
  Character *character_;
  Rectangle *rectangle_;
  std::vector<std::vector<std::list<std::pair<int, int>>>> grid_;
  std::unordered_map<std::string, std::vector<int>> pre_position_;

  std::string MakeID(int type, int index);
  std::vector<int> SearchLocation(Rectangle rectangle);
  void AddToGrid(int type, int index, std::vector<int> position);
  void RemoveFromGrid(int type, int index, std::vector<int> position);
  void UpdateToGrid(int type, int index, Rectangle rectangle);
  int CheckCollision(Rectangle rect1, Rectangle rect2);

public:
  Collision(float width, float height);
  void AddCharacter(Character *character);
  void AddRectangle(Rectangle *rectangle);
  void Check();
  void PrintGrid();
};

int main() {
  InitWindow(3000, 1000, "super mario");
  SetTargetFPS(60);
  Character character(5);
  character.SetCharacter(MARIO, {200, 300});
  Rectangle rect1 = {2000, 400, 100, 100};
  Collision collision(3000.0, 1000.0);
  collision.AddCharacter(&character);
  collision.AddRectangle(&rect1);

  // collision.PrintGrid();

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(BLACK);
    DrawRectangleRec(rect1, BLUE);

    collision.Check();

    Rectangle rect = character.GetRectangle();
    if (rect.y + rect.height >= 500)
      character.StopY(500);

    character.SetFrameCount();
    character.Update();
    character.Draw();

    if (IsKeyDown(KEY_LEFT))
      character.Run(true);
    if (IsKeyDown(KEY_RIGHT))
      character.Run(false);
    if (IsKeyDown(KEY_UP))
      character.Jump();
    if (IsKeyDown(KEY_D))
      character.Die();
    if (IsKeyDown(KEY_E))
      character.Evolve();

    EndDrawing();
  }

  CloseWindow();
  return 0;
}

std::string Collision::MakeID(int type, int index) {
  return std::to_string(type) + "," + std::to_string(index);
}

std::vector<int> Collision::SearchLocation(Rectangle rectangle) {
  int x1  = std::floor(rectangle.x / cellSize_),
      y1  = std::floor(rectangle.y / cellSize_), x2, y2;
  float a = (rectangle.x + rectangle.width) / cellSize_,
        b = (rectangle.y + rectangle.height) / cellSize_;
  if (a == std::floor(a)) {
    x2 = a;
  } else
    x2 = std::floor(a);
  if (b == std::floor(b)) {
    y2 = b;
  } else
    y2 = std::floor(b);

  if (x1 < 0)
    x1 = 0;
  if (y1 < 0)
    y1 = 0;
  if (x2 >= column_)
    x2 = column_ - 1;
  if (y2 >= row_)
    y2 = row_ - 1;

  return {y1, y2, x1, x2};
}

Collision::Collision(float width, float height) : cellSize_(16 * 4) {
  column_ = std::ceil(width / cellSize_);
  row_    = std::ceil(height / cellSize_);

  character_ = nullptr;

  grid_.resize(row_);
  for (int i = 0; i < grid_.size(); ++i) {
    grid_[i].resize(column_);
    for (int j = 0; j < grid_[i].size(); ++j) {
      grid_[i][j].clear();
    }
  }
  pre_position_.clear();
}
void Collision::AddCharacter(Character *character) {
  character_ = character;
}
void Collision::AddRectangle(Rectangle *rectangle) {
  rectangle_ = rectangle;
}
void Collision::AddToGrid(int type, int index, std::vector<int> position) {
  if (position.size() != 4)
    throw std::runtime_error("invalid position vector\n");
  for (int i = position[0]; i <= position[1]; ++i) {
    for (int j = position[2]; j <= position[3]; ++j) {
      grid_[i][j].push_back({type, index});
    }
  }
}
void Collision::RemoveFromGrid(int type, int index, std::vector<int> position) {
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

void Collision::UpdateToGrid(int type, int index, Rectangle rectangle) {
  std::string s             = MakeID(type, index);
  std::vector<int> position = SearchLocation(rectangle);
  bool different_position   = true;

  if (pre_position_.find(s) == pre_position_.end()) {
    pre_position_.insert({s, position});
  } else {
    if (position == pre_position_[s]) {
      different_position = false;
    } else {
      RemoveFromGrid(type, index, pre_position_[s]);
      pre_position_[s] = position;
    }
  }

  if (different_position)
    AddToGrid(type, index, position);
}
void Collision::Check() {
  Rectangle rect = character_->GetRectangle();
  Vector2 speed  = character_->GetSpeed();
  /*std::cout << rect.x << " " << speed.x << " " << rect.width << " " << rect.y
            << '\n';*/
  UpdateToGrid(2, 0, *rectangle_);

  std::vector<int> position = SearchLocation(rect);
  /*std::cout << position[0] << " " << position[1] << " " << position[2] << " "
            << position[3] << '\n';*/

  for (int i = position[0]; i <= position[1]; ++i) {
    auto it = grid_[i][position[2]].begin();
    while (it != grid_[i][position[2]].end()) {
      if (it->first == 2) {

        if (
          rect.x + speed.x <= rectangle_->x + rectangle_->width
          && !(
            rect.y >= rectangle_->y + rectangle_->height
            || rect.y + rect.height <= rectangle_->y)) {
          character_->StopX();
        }
      }
      ++it;
    }

    it = grid_[i][position[3]].begin();
    while (it != grid_[i][position[3]].end()) {
      if (it->first == 2) {
        if (
          rect.x + rect.width + speed.x >= rectangle_->x
          && !(
            rect.y >= rectangle_->y + rectangle_->height
            || rect.y + rect.height <= rectangle_->y)) {
          character_->StopX();
        }
      }
      ++it;
    }
  }

  for (int i = position[2]; i <= position[3]; ++i) {
    auto it = grid_[position[0]][i].begin();
    while (it != grid_[position[0]][i].end()) {
      if (it->first == 2) {
        if (rect.y + speed.y <= rectangle_->y + rectangle_->height && (rect.x +
rect.width >= rectangle_->x
          && rect.x <= rectangle_->x + rectangle_->width)) {
          character_->StopY();
        }
      }
      ++it;
    }

    it = grid_[position[1]][i].begin();
    while (it != grid_[position[1]][i].end()) {
      if (it->first == 2) {
        if (
          rect.y + rect.height >= rectangle_->y && (rect.x + rect.width >=
rectangle_->x
          && rect.x <= rectangle_->x + rectangle_->width)) {
          character_->StopY(rectangle_->y);
        }
      }
      ++it;
    }
  }
}
void Collision::PrintGrid() {
  std::cout << row_ << " " << column_ << '\n';
  for (int i = 0; i < grid_.size(); ++i) {
    for (int j = 0; j < grid_[i].size(); ++j) {
      std::cout << grid_[i][j].size() << " ";
    }
    std::cout << '\n';
  }
}
int Collision::CheckCollision(Rectangle r1, Rectangle r2) {
  if (
    r1.x <= r2.x + r2.width && r1.x + r1.width >= r2.x
    && r1.y <= r2.y + r2.height && r1.y + r1.height >= r2.y)
    return 1;
  else if (r1.y <= r2.y + r2.height && r1.y + r1.height >= r2.y)
    return 2;
  else
    return 0;
}