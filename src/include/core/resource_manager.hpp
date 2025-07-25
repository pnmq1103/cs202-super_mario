#pragma once
#include <algorithm>
#include <filesystem>
#include <raylib.h>
#include <string>
#include <unordered_map>
#include <vector>
#include <variant>
#include "tileson.hpp"
#include "../blocks/block.hpp"
//#include "include/core/file_handler.hpp"

namespace fs = std::filesystem;

struct TilesetInfo {
  int firstGid, columns, margin, spacing;
  Vector2 tileSize;
};

struct BlockInfo {
  int gid;
  Vector2 pos;
  Vector2 size;
  int type;
  bool solid;
  bool animating;
};

struct EnemyInfo {
  int gid;
  Vector2 pos;
  Vector2 size;
  Vector2 velocity;
  int type;
};

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
  //std::unordered_map<std::string, Texture> backgrounds;
  Texture background_ground;

  std::unordered_map<std::string, Music> musics;
  std::unordered_map<std::string, Sound> sounds;

  //to store textures parsed from map
  std::map<int, Texture> tilesetMapStore;
  //to store block info
  std::vector<BlockInfo> blockInfoMapStore;
  //to store backgrounds
  Texture background;
  //to store enemies
  std::vector<EnemyInfo> enemyMapStore;


public:
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
  void LoadMap(const std::string &path);
  
  //getters for preloaded resources
  Texture GetMario(char type, int idx);
  Texture GetLuigi(char type, int idx);
  Texture GetEnemy(int idx);
  Texture GetTile(int idx);
  Texture GetIcon(int idx);
  Texture GetBackground(int idx);
  Music GetMusic(std::string key);
  Sound GetSound(std::string key);

  //getters for maps
  std::map<int, Texture> GetTilesetMap() const;
  std::vector<BlockInfo> GetBlocksMap() const;
  Texture GetBackgroundMap() const;
  std::vector<EnemyInfo> GetEnemiesMap() const;
};