#include <iostream>
#include <raylib.h>
#include <string>

#include "include/core/media.hpp"

Media::~Media() {
  for (auto &[_, sound] : sounds_)
    UnloadSound(sound);
  for (auto &[_, music] : musics_) {
    StopMusicStream(music);
    UnloadMusicStream(music);
  }
}

void Media::Init() {
  LoadMusic();
  LoadSounds();
}

void Media::LoadMusic() {
  auto Load = [this](const std::string &name) {
    musics_[name] = LoadMusicStream(("res/musics/" + name + ".ogg").c_str());
  };
  for (const auto &music : music_names_)
    Load(music);
}

void Media::LoadSounds() {
  auto Load = [this](const std::string &name) {
    sounds_[name] = LoadSound(("res/sounds/" + name + ".wav").c_str());
  };
  for (const auto &sound : sound_names_)
    Load(sound);
}

void Media::PlaySound(const std::string &name) {
  if (sounds_.find(name) == sounds_.end())
    return;

  RAYLIB_H::PlaySound(sounds_.at(name));
}

void Media::SetSoundVolume(float volume) {
  for (auto &[_, sound] : sounds_)
    RAYLIB_H::SetSoundVolume(sound, volume);
}

void Media::PlayMusic(const std::string &name) {
  if (musics_.find(name) == musics_.end())
    return;

  if (IsMusicValid(musics_[name])) {
    PlayMusicStream(musics_.at(name));
    cur_music_ = name;
  }
}

void Media::UpdateMusic() {
  if (musics_.find(cur_music_) == musics_.end())
    return;

  UpdateMusicStream(musics_.at(cur_music_));
}

void Media::TogglePauseMusic() {
  if (musics_.find(cur_music_) == musics_.end())
    return;

  if (IsMusicStreamPlaying(musics_.at(cur_music_)))
    PauseMusicStream(musics_.at(cur_music_));
  else
    ResumeMusicStream(musics_.at(cur_music_));
}

void Media::StopMusic() {
  if (musics_.find(cur_music_) == musics_.end())
    return;

  if (IsMusicStreamPlaying(musics_.at(cur_music_))) {
    StopMusicStream(musics_.at(cur_music_));
    cur_music_ = "";
  }
}

void Media::SetMusicVolume(float volume) {
  for (auto &[_, music] : musics_)
    RAYLIB_H::SetMusicVolume(music, volume);
}

void Media::SaveMusicState() {
  if (musics_.find(cur_music_) == musics_.end())
    return;

  if (IsMusicStreamPlaying(musics_.at(cur_music_)))
    music_state_.emplace(
      cur_music_, GetMusicTimePlayed(musics_.at(cur_music_)));
}

void Media::LoadMusicState() {
  if (!music_state_.empty()) {
    auto [name, time] = music_state_.top();
    music_state_.pop();
    SeekMusicStream(musics_.at(name), time);
    PlayMusic(name);
  }
}

size_t Media::MusicStateSize() {
  return music_state_.size();
}
