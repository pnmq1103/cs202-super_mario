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

  sprite_sheets_.reserve(3);
}

Map::~Map() {}

#include <iostream>

void Map::Init() {
  try {
    sprite_sheets_ = {
      {1, 73, "tileset_ground", "res/sprites/tilesets/tileset_ground.txt"},
      // unprocessed tileset_underground
      //{74, 73, "tileset_underground",
      //"res/sprites/tilesets/tileset_underground.txt"},
      {74, 16, "bowser", "res/sprites/enemies/bowser.txt"},
      {90, 23, "minions", "res/sprites/enemies/minions.txt"}};
  } catch (const std::exception &e) {
    std::cerr << "Error in Map::Init: " << e.what() << std::endl;
    throw;
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
  mp["enemies"]             = &App.Resource().GetEnemy('i');

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

Rectangle Map::GetInfo(int gidx) const {
  for (const auto &sheet : sprite_sheets_) {
    if (gidx >= sheet.first_gidx_ && gidx < sheet.first_gidx_ + sheet.count_) {
      return sheet.info.at(gidx - sheet.first_gidx_);
    }
  }
  throw std::out_of_range("global index not found");
}
