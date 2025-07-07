#pragma once
#include "raylib.h"
#include "file_handler.hpp"
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

namespace fs = std::filesystem;


class ResManager {
  static std::map<std::string, Texture2D> mario_normal;
  static std::map<std::string, Texture2D> mario_star;
  static std::map<std::string, Texture2D> mario_fire;
  static std::map<std::string, Texture2D> luigi_normal;
  static std::map<std::string, Texture2D> luigi_star;
  static std::map<std::string, Texture2D> luigi_fire;
  static std::map<std::string, Texture2D> luigi_electric;
  static std::map<std::string, Texture2D> enemies;
  static std::map<std::string, Texture2D> icons;
  static std::map<std::string, Texture2D> tileset;
  static std::map<std::string, Texture2D> backgrounds;

  static std::map<std::string, Music> musics;
  static std::map<std::string, Sound> sounds;

  const fs::path musicPath = "res/musics";
  const fs::path soundPath = "res/sounds";

  void LoadHelper(
    const fs::path &imgPath, const fs::path &coorPath, std::string keyPrefix,
    std::map<std::string, Texture2D>& map);

  void LoadTexture(
    const fs::path &imgPath, const fs::path &coorPath, std::string keyPrefix,
    std::map<std::string, Texture2D>& textMap);

  void LoadMusic();

  void LoadSounds();

public:
  ResManager();

  SaveDatawMap LoadResourcesFromFile();

  static bool SaveResourcesToFile(const SaveDatawMap &data);

  static Texture2D GetMario(char type, int index);

  static Texture2D GetLuigi(char type, int index);

  static Texture2D GetEnemy(int index);

  static Texture2D GetTile(int index);

  static Texture2D GetIcon(int index);

  static Texture2D GetBackground(int index);

  static Music GetMusic(std::string key);

  static Sound GetSound(std::string key);

  void Shutdown();

  ~ResManager();
};