#include "include/core/resource_manager.hpp"
#include <array>
#include <stdexcept>

ResManager::~ResManager() {
  for (auto &texture : textures_)
    UnloadTexture(texture.second);

  for (auto &sound : sounds)
    UnloadSound(sound.second);

  for (auto &music : musics) {
    StopMusicStream(music.second);
    UnloadMusicStream(music.second);
  }
}

void ResManager::Init() {
  LoadTextures();
  LoadSounds();
  LoadMusic();
}

void ResManager::LoadTextures() {
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

  textures_["icons"]   = LoadTexture("res/sprites/icons/icons.png");
  textures_["objects"] = LoadTexture("res/sprites/icons/objects.png");
  textures_["electric_shot"]
    = LoadTexture("res/sprites/electric_shot/electric_shot.png");

  textures_["tileset_ground"]
    = LoadTexture("res/sprites/tilesets/tileset_ground.png");
  textures_["tileset_underground"]
    = LoadTexture("res/sprites/tilesets/tileset_underground.png");

  textures_["backgrounds"]
    = LoadTexture("res/sprites/backgrounds/background_ground.png");
}

void ResManager::LoadMusic() {
  auto Load = [this](const std::string &name) {
    musics[name] = LoadMusicStream(("res/musics/" + name + ".ogg").c_str());
  };

  for (const auto &music : music_names)
    Load(music);
}

void ResManager::LoadSounds() {
  auto Load = [this](const std::string &name) {
    sounds[name] = LoadSound(("res/sounds/" + name + ".wav").c_str());
  };

  for (const auto &sound : sound_names)
    Load(sound);
}

const Texture &ResManager::GetMario(char type) const {
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

const Texture &ResManager::GetLuigi(char type) const {
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

const Texture &ResManager::GetEnemy(char type) const {
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

const Texture &ResManager::GetTileset(char type) const {
  switch (type) {
    case 'g':
      return textures_.at("tileset_ground");
    case 'u':
      return textures_.at("tileset_underground");
    default:
      throw std::runtime_error("Invalid tileset type");
  }
}

const Texture &ResManager::GetIcon() const {
  return textures_.at("icons");
}

const Texture &ResManager::GetObject() const {
  return textures_.at("objects");
}

const Texture &ResManager::GetElectricShot() const {
  return textures_.at("electric_shot");
}

const Texture &ResManager::GetBackground() const {
  return textures_.at("backgrounds");
}

const Music &ResManager::GetMusic(std::string key) const {
  auto it = musics.find(key);
  if (it == musics.end())
    throw std::out_of_range("Missing music");
  return it->second;
}

const Sound &ResManager::GetSound(std::string key) const {
  auto it = sounds.find(key);
  if (it == sounds.end())
    throw std::out_of_range("Missing sound");
  return it->second;
}