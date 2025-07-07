#pragma once
#include <raylib.h>
#include <stack>
#include <string>
#include <unordered_map>
#include <utility>

class Media {
private:
  std::unordered_map<std::string, Music> musics_;
  std::unordered_map<std::string, Sound> sounds_;
  std::string cur_music_;

  // Music name, played time
  std::stack<std::pair<std::string, float>> music_state_;

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
};