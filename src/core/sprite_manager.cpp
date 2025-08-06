#include "include/core/sprite_manager.hpp"
#include <array>
#include <stdexcept>

SpriteManager::~SpriteManager() {
  for (auto &[_, texture] : textures_)
    UnloadTexture(texture);
}

void SpriteManager::Init() {
  textures_["mario_normal"]
    = LoadTexture("res/sprites/characters/mario_normal.png");
  textures_["mario_star"]
    = LoadTexture("res/sprites/characters/mario_star.png");
  textures_["mario_fire"]
    = LoadTexture("res/sprites/characters/mario_fire.png");

  textures_["luigi_normal"]
    = LoadTexture("res/sprites/characters/luigi_normal.png");
  textures_["luigi_star"]
    = LoadTexture("res/sprites/characters/luigi_star.png");
  textures_["luigi_fire"]
    = LoadTexture("res/sprites/characters/luigi_fire.png");
  textures_["luigi_electric"]
    = LoadTexture("res/sprites/characters/luigi_electric.png");

  textures_["bowser"]  = LoadTexture("res/sprites/enemies/bowser.png");
  textures_["minions"] = LoadTexture("res/sprites/enemies/minions.png");
  textures_["enemies_icon"]
    = LoadTexture("res/sprites/enemies/enemies_icon.png");

  textures_["objects"] = LoadTexture("res/sprites/objects/block_objects.png");
  textures_["electric_shot"]
    = LoadTexture("res/sprites/electric_shot/electric_shot.png");

  textures_["tileset_ground"]
    = LoadTexture("res/sprites/tilesets/tileset_ground.png");
  textures_["tileset_underground"]
    = LoadTexture("res/sprites/tilesets/tileset_underground.png");
  textures_["tileset_minimal"]
    = LoadTexture("res/sprites/tilesets/tileset_minimal.png");
  textures_["backgrounds"]
    = LoadTexture("res/sprites/backgrounds/background_ground.png");
}

const Texture &SpriteManager::GetMario(char type) const {
  switch (type) {
    case 'n':
      return textures_.at("mario_normal");
    case 's':
      return textures_.at("mario_star");
    case 'f':
      return textures_.at("mario_fire");
    default:
      throw std::runtime_error("Invalid Mario type");
  }
}

const Texture &SpriteManager::GetLuigi(char type) const {
  switch (type) {
    case 'n':
      return textures_.at("luigi_normal");
    case 's':
      return textures_.at("luigi_star");
    case 'f':
      return textures_.at("luigi_fire");
    case 'e':
      return textures_.at("luigi_electric");
    default:
      throw std::runtime_error("Invalid Luigi type");
  }
}

const Texture &SpriteManager::GetEnemy(char type) const {
  switch (type) {
    case 'b':
      return textures_.at("bowser");
    case 'm':
      return textures_.at("minions");
    case 'i':
      return textures_.at("enemies_icon");
    default:
      throw std::runtime_error("Invalid enemy type");
  }
}

const Texture &SpriteManager::GetTileset(char type) const {
  switch (type) {
    case 'g':
      return textures_.at("tileset_ground");
    case 'u':
      return textures_.at("tileset_underground");
    case 'm':
      return textures_.at("tileset_minimal");
    default:
      throw std::runtime_error("Invalid tileset type");
  }
}

const Texture &SpriteManager::GetObject() const {
  return textures_.at("objects");
}

const Texture &SpriteManager::GetElectricShot() const {
  return textures_.at("electric_shot");
}

const Texture &SpriteManager::GetBackground() const {
  return textures_.at("backgrounds");
}