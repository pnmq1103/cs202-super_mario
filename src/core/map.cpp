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
      {1, 73, "tileset_ground", "res/sprites/tilesets/tileset_ground.txt"},
      {74, 71, "tileset_underground",
       "res/sprites/tilesets/tileset_underground.txt"},
      {145, 16, "bowser", "res/sprites/enemies/bowser.txt"},
      {161, 23, "minions", "res/sprites/enemies/minions.txt"},
      {184, 4, "enemies_icon", "res/sprites/enemies/enemies_icon.txt"}};
  } catch (const std::exception &e) {
    std::cerr << "Map::Init: " << e.what() << std::endl;
  }
}

void Map::SetTile(int pos, int gidx) {
  layers_[static_cast<size_t>(MapLayer::Tile1)].at(pos) = gidx;
}

void Map::SetEnemy(int pos, int gidx) {
  layers_[static_cast<size_t>(MapLayer::Objects)].at(pos) = gidx;
}

void Map::SetPipe(int pos, int gidx) {
  layers_[static_cast<size_t>(MapLayer::Tile2)].at(pos) = gidx;
}

int Map::GetTile(int pos) const {
  return layers_[static_cast<size_t>(MapLayer::Tile1)].at(pos);
}

int Map::GetEnemy(int pos) const {
  return layers_[static_cast<size_t>(MapLayer::Objects)].at(pos);
}

int Map::GetPipe(int pos) const {
  return layers_[static_cast<size_t>(MapLayer::Tile2)].at(pos);
}

const Texture &Map::FindTexture(std::string texture_key) const {
  std::unordered_map<std::string, const Texture *> mp;
  mp["tileset_ground"]      = &App.Resource().GetTileset('g');
  mp["tileset_underground"] = &App.Resource().GetTileset('u');
  mp["bowser"]              = &App.Resource().GetEnemy('b');
  mp["minions"]             = &App.Resource().GetEnemy('m');
  mp["enemies_icon"]        = &App.Resource().GetEnemy('i');

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
  throw std::out_of_range("global index not found");
}

Rectangle Map::GetInfo(int gidx) const {
  for (const auto &sheet : sprite_sheets_) {
    if (gidx >= sheet.first_gidx && gidx < sheet.first_gidx + sheet.count) {
      return sheet.info.at(gidx - sheet.first_gidx);
    }
  }
  throw std::out_of_range("global index not found");
}
