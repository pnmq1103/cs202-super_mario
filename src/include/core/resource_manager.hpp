#pragma once
#include "file_handler.hpp"
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <raylib.h>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

namespace fs = std::filesystem;

class ResManager {
  static std::unordered_map<std::string, Texture> mario_normal;
  static std::unordered_map<std::string, Texture> mario_star;
  static std::unordered_map<std::string, Texture> mario_fire;
  static std::unordered_map<std::string, Texture> luigi_normal;
  static std::unordered_map<std::string, Texture> luigi_star;
  static std::unordered_map<std::string, Texture> luigi_fire;
  static std::unordered_map<std::string, Texture> luigi_electric;
  static std::unordered_map<std::string, Texture> enemies;
  static std::unordered_map<std::string, Texture> icons;
  static std::unordered_map<std::string, Texture> tileset;
  static std::unordered_map<std::string, Texture> backgrounds;

  static std::unordered_map<std::string, Music> musics;
  static std::unordered_map<std::string, Sound> sounds;

  void LoadHelper(
    const fs::path &imgPath, const fs::path &coorPath, std::string keyPrefix,
    std::unordered_map<std::string, Texture> &unordered_map);

  void LoadTexture(
    const fs::path &imgPath, const fs::path &coorPath, std::string keyPrefix,
    std::unordered_map<std::string, Texture> &textMap);

  void LoadMusic();

  void LoadSounds();

public:
  ~ResManager();
  ResManager();

  void Shutdown();

  SaveDatawMap LoadResourcesFromFile();

  static bool SaveResourcesToFile(const SaveDatawMap &data);

  static Texture GetMario(char type, int index);

  static Texture GetLuigi(char type, int index);

  static Texture GetEnemy(int index);

  static Texture GetTile(int index);

  static Texture GetIcon(int index);

  static Texture GetBackground(int index);

  static Music GetMusic(std::string key);

  static Sound GetSound(std::string key);
};