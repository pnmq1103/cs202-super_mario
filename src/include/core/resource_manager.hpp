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



class ResManager {
  static std::map<std::string, Texture2D> characters;
  static std::map<std::string, Texture2D> blocks;
  static std::map<std::string, Music> musics;
  static std::map<std::string, Sound> sounds;

  std::vector<std::pair<Vector2, Vector2>> coors;
  std::map<std::string, Texture2D> tempText;
  const std::string imgPath = "res\\";
  const std::string coorPath  = "res\\";
  const std::string musicPath = "res\\musics\\";
  const std::string soundPath = "res\\sounds\\";

  
  void loadTexture(std::string imgPath, std::string coorPath, std::string keyPrefix);

  void loadCharacters();

  void loadBlocks();

  void loadMusic();

  void loadSounds();

  public:

    ResManager();

    SaveDatawMap loadResourcesFromFile();

    static bool saveResourcesToFile(const SaveDatawMap& data);

    static Texture2D getCharacter(std::string key);

    static Texture2D getBlock(std::string key);

    static Music getMusic(std::string key);
    
    static Sound getSound(std::string key);

    ~ResManager();

};