#pragma once
#include <filesystem>
#include <raylib.h>
#include <string>
#include <unordered_map>

#include "include/core/file_handler.hpp"

namespace fs = std::filesystem;

class ResManager {
  std::unordered_map<std::string, Texture> mario_normal;
  std::unordered_map<std::string, Texture> mario_star;
  std::unordered_map<std::string, Texture> mario_fire;
  std::unordered_map<std::string, Texture> luigi_normal;
  std::unordered_map<std::string, Texture> luigi_star;
  std::unordered_map<std::string, Texture> luigi_fire;
  std::unordered_map<std::string, Texture> luigi_electric;
  std::unordered_map<std::string, Texture> enemies;
  std::unordered_map<std::string, Texture> icons;
  std::unordered_map<std::string, Texture> tileset;
  std::unordered_map<std::string, Texture> backgrounds;

  std::unordered_map<std::string, Music> musics;
  std::unordered_map<std::string, Sound> sounds;

private:
  void LoadHelper(
    const fs::path &img_path, std::string prefix,
    std::unordered_map<std::string, Texture> &texture_map);

  void LoadTextures();
  void LoadMusic();
  void LoadSounds();

public:
  ~ResManager();
  ResManager();

  void Init();

  // SaveData LoadResourcesFromFile();
  // bool SaveResourcesToFile(const SaveData &data);

  Texture GetMario(char type, int idx);
  Texture GetLuigi(char type, int idx);
  Texture GetEnemy(int idx);
  Texture GetTile(int idx);
  Texture GetIcon(int idx);
  Texture GetBackground(int idx);
  Music GetMusic(std::string key);
  Sound GetSound(std::string key);
};