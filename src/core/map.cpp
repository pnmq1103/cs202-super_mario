#include <iostream>
#include <stdexcept>

#include "include/core/application.hpp"
#include "include/core/constants.hpp"
#include "include/core/map.hpp"

Map::Map() {
  layers_.resize(3);
  layers_[static_cast<size_t>(MapLayer::Tile1)].resize(
    constants::mapWidth * constants::mapHeight, 0);
  layers_[static_cast<size_t>(MapLayer::Objects)].resize(
    constants::mapWidth * constants::mapHeight, 0);
  layers_[static_cast<size_t>(MapLayer::Tile2)].resize(
    constants::mapWidth * constants::mapHeight, 0);

  sprite_sheets_.reserve(5);
}

Map::~Map() {}

void Map::Init() {
  try {
    sprite_sheets_ = {
      {1, 37, "tileset_minimal", "res/sprites/tilesets/tileset_minimal.txt"},
      {38, 18, "bowser", "res/sprites/enemies/bowser.txt"},
      {56, 23, "minions", "res/sprites/enemies/minions.txt"},
      {79, 4, "enemies_icon", "res/sprites/enemies/enemies_icon.txt"}};
  } catch (const std::exception &e) {
    std::cerr << "Map::Init: " << e.what() << std::endl;
  }
}

void Map::SetCell(MapLayer layer, int pos, int gidx) {
  layers_[static_cast<size_t>(layer)].at(pos) = gidx;
}

int Map::GetCell(MapLayer layer, int pos) const {
  return layers_[static_cast<size_t>(layer)].at(pos);
}

const Texture &Map::FindTexture(std::string texture_key) const {
  std::unordered_map<std::string, const Texture *> mp;
  mp["tileset_minimal"] = &App.Resource().GetTileset('m');
  mp["bowser"]          = &App.Resource().GetEnemy('b');
  mp["minions"]         = &App.Resource().GetEnemy('m');
  mp["enemies_icon"]    = &App.Resource().GetEnemy('i');

  for (const auto &[key, texture] : mp) {
    if (key == texture_key)
      return *mp[key];
  }
  throw std::out_of_range("texture not found");
}

const Texture &Map::GetTexture(int gidx) const {
  for (const auto &sheet : sprite_sheets_) {
    if (gidx >= sheet.first_gidx && gidx < sheet.first_gidx + sheet.count)
      return FindTexture(sheet.texture_key);
  }
  throw std::out_of_range(
    "global index at" + std::to_string(gidx) + "not found");
}

Rectangle Map::GetInfo(int gidx) const {
  for (const auto &sheet : sprite_sheets_) {
    if (gidx >= sheet.first_gidx && gidx < sheet.first_gidx + sheet.count) {
      return sheet.info.at(gidx - sheet.first_gidx);
    }
  }
  throw std::out_of_range(
    " get info global index at" + std::to_string(gidx) + "not found");
}
