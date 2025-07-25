#pragma once
#include <raylib.h>
#include <unordered_map>
#include <vector>

#include "include/core/file_handler.hpp"

struct SpriteSheet {
  int first_gidx_         = {0};
  int count_              = {0};
  const Texture *texture_ = {nullptr};
  std::unordered_map<int, Rectangle> info;

  SpriteSheet(
    int first_gidx, int count, const Texture &texture, std::string path)
      : first_gidx_(first_gidx), count_(count), texture_(&texture) {
    FileHandler::ReadSpriteInfo(path, info);
  }
};

class Map {
private:
  int block_width_  = {50};
  int block_height_ = {25};
  std::vector<SpriteSheet> sprite_sheets_;

  std::vector<int> tile_layer_;
  std::vector<int> enemy_layer_;

public:
  Map(int block_width, int block_height);
  ~Map();

  void Init();
  int FindLocalIndex(int gidx);
  void SetTile(int pos, int gidx);
  void SetEnemy(int pos, int gidx);
  int GetTile(int pos);
  int GetEnemy(int pos);
  const Texture &GetTexture(int gidx) const;
  Rectangle GetInfo(int gidx);
};