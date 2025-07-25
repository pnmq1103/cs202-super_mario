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
    {1, 10, "backgrounds", "res/sprites/backgrounds/backgrounds.txt"},
    {11, 144, "tileset_ground", "res/sprites/tilesets/tileset_ground.txt"},
    {155, 144, "tileset_underground",
     "res/sprites/tilesets/tileset_underground.txt"},
    {299, 5, "enemies", "res/sprites/enemies/enemies.txt"}
    // need to create another sprite
  };
}

void Map::SetTile(int pos, int gidx) {
  if (pos < 0 || pos > block_width_ * block_height_)
    throw std::out_of_range("invalid index");
  tile_layer_.at(pos) = gidx;
}

void Map::SetEnemy(int pos, int gidx) {
  if (pos < 0 || pos > block_width_ * block_height_)
    throw std::out_of_range("invalid index");
  enemy_layer_.at(pos) = gidx;
}

int Map::GetTile(int pos) const {
  if (pos < 0 || pos > block_width_ * block_height_)
    throw std::out_of_range("invalid index");
  return tile_layer_.at(pos);
}

int Map::GetEnemy(int pos) const {
  if (pos < 0 || pos > block_width_ * block_height_)
    throw std::out_of_range("invalid index");
  return enemy_layer_.at(pos);
}

const Texture &Map::FindTexture(std::string texture_key) const {
  std::unordered_map<std::string, const Texture *> mp;
  mp["backgrounds"]         = &App.Resource().GetBackground();
  mp["tileset_ground"]      = &App.Resource().GetTileset('g');
  mp["tileset_underground"] = &App.Resource().GetTileset('u');
  mp["enemies"]             = &App.Resource().GetEnemy();

  for (const auto &[key, texture] : mp) {
    if (key == texture_key)
      return *mp[key];
  }
  throw std::out_of_range("texture not found");
}

const Texture &Map::GetTexture(int gidx) const {
  for (const auto &sheet : sprite_sheets_) {
    if (gidx >= sheet.first_gidx_ && gidx < sheet.first_gidx_ + sheet.count_)
      return FindTexture(sheet.texture_key_);
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
