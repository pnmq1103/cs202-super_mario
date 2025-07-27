#pragma once
#include <algorithm>
#include <array>
#include <raylib.h>
#include <string>
#include <unordered_map>
#include <vector>

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

  // to store textures parsed from map
  // std::map<int, Texture> tilesetMapStore;

private:
  void LoadTextures();
  void LoadMusic();
  void LoadSounds();

public:
  ~ResManager();
  ResManager();

  void Init();

  // SaveData LoadResourcesFromFile();
  // bool SaveResourcesToFile(const SaveData &data);
  // bool LoadMap(const std::string &path, std::vector<Block *> &blockData);

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
};