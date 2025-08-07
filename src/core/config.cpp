#include <raylib.h>
#include <raymath.h>

#include "include/core/config.hpp"

void Config::Init() {}

void Config::Reset() {
  music_volume_ = 1;
  sound_volume_ = 1;
}

float Config::GetMusicVolume() {
  return music_volume_;
}

float Config::GetSoundVolume() {
  return sound_volume_;
}

void Config::SetMusicVolume(float volume) {
  music_volume_ = volume;
  music_volume_ = Clamp(music_volume_, 0, 1);
}

void Config::SetSoundVolume(float volume) {
  sound_volume_ = volume;
  sound_volume_ = Clamp(sound_volume_, 0, 1);
}