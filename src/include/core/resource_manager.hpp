#pragma once
#include "raylib.h"
#include "..\include\core\file_handler.hpp"
#include <map>
#include <string>
#include <fstream>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

class ResManager {
  static std::map<std::string, Texture2D> characters;
  static std::map<std::string, Texture2D> blocks;
  static std::map<std::string, Music> musics;
  static std::map<std::string, Sound> sounds;
  static std::map<std::string, Texture2D> backgrounds;

  std::vector<std::pair<Vector2, Vector2>> coors;
  std::map<std::string, Texture2D> tempText;
  const fs::path imgPath   = "res";
  const fs::path coorPath  = "res";
  const fs::path musicPath = "res/musics";
  const fs::path soundPath = "res/sounds";

  
  void loadTexture(const fs::path& imgPath, const fs::path& coorPath, std::string keyPrefix);

  void loadCharacters();

  void loadBlocks();

  void loadBackgrounds();

  void loadMusic();

  void loadSounds();

  public:

    ResManager();

    SaveDatawMap loadResourcesFromFile();

    static bool saveResourcesToFile(const SaveDatawMap& data);

    static Texture2D getCharacter(std::string key);

    static Texture2D getBlock(std::string key);

    static Texture2D getBackground(int index);

    static Music getMusic(std::string key);
    
    static Sound getSound(std::string key);

    ~ResManager();

};