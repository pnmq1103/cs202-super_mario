#pragma once
#include <algorithm>
#include <array>
#include <raylib.h>
#include <string>
#include <unordered_map>
#include <map>
#include <vector>
#include <variant>
#include "include/blocks/block.hpp"
#include "include/external/tileson.hpp"
#include <filesystem>
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
  std::unordered_map<std::string, Texture> textures_;

  std::unordered_map<std::string, Music> musics;
  std::array<std::string, 13> music_names = {
    "bonus",
    "boss",
    "bowser_battle",
    "castle_theme",
    "choose_character",
    "ending",
    "final_battle",
    "ground_theme",
    "invincibility_theme",
    "overworld",
    "title",
    "underground_theme",
    "underwater_theme"};

  std::unordered_map<std::string, Sound> sounds;
  std::array<std::string, 28> sound_names = {
    "1up",
    "beep",
    "billfirework",
    "bowserfall",
    "brick",
    "bump",
    "castle_complete",
    "coin",
    "course_clear",
    "fire",
    "fireball",
    "flagpole",
    "gameover",
    "gameover_unused",
    "hurryup",
    "item",
    "jump",
    "jumpsmall",
    "kickkill",
    "level_complete",
    "life_lost",
    "pause",
    "pipepowerdown",
    "powerup",
    "stompswim",
    "time-up_warning",
    "vine",
    "world_clear",
  };

    private:
 //to store textures parsed from map
  std::map<int, Texture> tilesetMapStore;
  //to store block info
  std::vector<BlockInfo> blockInfoMapStore;
  //to store backgrounds
  Texture background;
  //to store enemies
  std::vector<EnemyInfo> enemyMapStore;

  //load all textures, music, and sounds
  void LoadTextures();
  void LoadMusic();
  void LoadSounds();

  public:

  ~ResManager();
  ResManager();

  void Init();

  // SaveData LoadResourcesFromFile();
  // bool SaveResourcesToFile(const SaveData &data);

  
  //getters for preloaded resources
  const Texture &GetMario(char type) const;
  const Texture &GetLuigi(char type) const;
  const Texture &GetEnemy() const;
  const Texture &GetTileset(char type) const;
  const Texture &GetIcon() const;
  const Texture &GetObject() const;
  const Texture &GetElectricShot() const;
  const Texture &GetBackground() const;
  const Music &GetMusic(std::string key) const;
  const Sound &GetSound(std::string key) const;

  //functions for a map from Tileson - backup code
  // 
  // void LoadMap(const std::string &path);
 /* std::map<int, Texture> GetTilesetMap() const;
  std::vector<BlockInfo> GetBlocksMap() const;
  Texture GetBackgroundMap() const;
  std::vector<EnemyInfo> GetEnemiesMap() const;*/
};