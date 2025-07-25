#include <stdexcept>

#include "include/core/application.hpp"
#include "include/core/map.hpp"

Map::Map(int block_width, int block_height)
    : block_width_(block_width), block_height_(block_height) {
  tile_layer_.resize(block_width_ * block_height_, 0);
  enemy_layer_.resize(block_width_ * block_height_, 0);
  sprite_sheets_.reserve(4);
}

Map::~Map() {}

void Map::Init() {

  sprite_sheets_ = {
    {1, 10, App.Resource().GetBackground(),
     "res/sprites/backgrounds/backgrounds.txt"},
    {11, 144, App.Resource().GetTileset('g'),
     "res/sprites/tilesets/tileset_ground.txt"},
    {155, 144, App.Resource().GetTileset('u'),
     "res/sprites/tilesets/tileset_underground.txt"},
    {299, 5, App.Resource().GetEnemy(), "res/sprites/enemies/enemies.txt"},
  };
}

int Map::FindLocalIndex(int gidx) {
  for (const auto &sheet : sprite_sheets_) {
    if (gidx >= sheet.first_gidx_ && gidx < sheet.first_gidx_ + sheet.count_)
      return gidx - sheet.first_gidx_;
  }
  throw std::out_of_range("global index not found");
}

void Map::SetTile(int pos, int gidx) {
  if (pos < 0 || pos > block_width_ * block_height_)
    throw std::out_of_range("invalid index");
  tile_layer_[pos] = gidx;
}

void Map::SetEnemy(int pos, int gidx) {
  if (pos < 0 || pos > block_width_ * block_height_)
    throw std::out_of_range("invalid index");
  enemy_layer_[pos] = gidx;
}

int Map::GetTile(int pos) {
  if (pos < 0 || pos > block_width_ * block_height_)
    throw std::out_of_range("invalid index");
  return tile_layer_[pos];
}

int Map::GetEnemy(int pos) {
  if (pos < 0 || pos > block_width_ * block_height_)
    throw std::out_of_range("invalid index");
  return enemy_layer_[pos];
}

const Texture &Map::GetTexture(int gidx) const {
  for (const auto &sheet : sprite_sheets_) {
    if (gidx >= sheet.first_gidx_ && gidx < sheet.first_gidx_ + sheet.count_)
      return *sheet.texture_;
  }
  throw std::out_of_range("global index not found");
}

Rectangle Map::GetInfo(int gidx) {
  for (const auto &sheet : sprite_sheets_) {
    if (gidx >= sheet.first_gidx_ && gidx < sheet.first_gidx_ + sheet.count_) {
      return sheet.info.at(gidx - sheet.first_gidx_);
    }
  }
  throw std::out_of_range("global index not found");
}
