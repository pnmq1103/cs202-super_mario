#pragma once
#include <array>
#include <raylib.h>
#include <stack>
#include <string>
#include <unordered_map>
#include <utility>

class Media {
private:
  std::string cur_music_;
  std::unordered_map<std::string, Music> musics_;
  std::array<std::string, 14> music_names_ = {
    "title",
    "ground_theme",
    "castle_theme",
    "underground_theme",
    "underwater_theme",
    "bonus",
    "boss",
    "bowser_battle",
    "choose_character",
    "ending",
    "final_battle",
    "invincibility_theme",
    "overworld",
    "level_end"};
  std::unordered_map<std::string, Sound> sounds_;
  std::array<std::string, 28> sound_names_ = {
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
  // Music name, played time
  std::stack<std::pair<std::string, float>> music_state_;

private:
  void LoadMusic();
  void LoadSounds();

public:
  Media() = default;
  ~Media();

  void Init();

  void PlaySound(const std::string &name);
  void SetSoundVolume(float volume);

  void PlayMusic(const std::string &name);
  void UpdateMusic();
  void TogglePauseMusic();
  void StopMusic();
  void SetMusicVolume(float volume);

  void SaveMusicState();
  void LoadMusicState();

  size_t MusicStateSize();
};