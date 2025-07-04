#include <raylib.h>
#include <string>

#include "../include/core/media.hpp"

Media::~Media() {
  for (const auto &[_, sound] : sounds_)
    UnloadSound(sound);
  for (const auto &[_, music] : musics_)
    UnloadMusicStream(music);
}

void Media::Init() {
  // Load Music
  musics_["title"]        = LoadMusicStream("res/musics/title.ogg");
  musics_["ground_theme"] = LoadMusicStream("res/musics/ground_theme.ogg");
  musics_["castle_theme"] = LoadMusicStream("res/musics/castle_theme.ogg");

  // Load Sound
  sounds_["beep"] = LoadSound("res/sounds/beep.wav");
}

void Media::PlaySound(const std::string &name) {
  if (sounds_.find(name) != sounds_.end())
    RAYLIB_H::PlaySound(sounds_[name]);
}

void Media::PlayMusic(const std::string &name) {
  if (musics_.find(name) != musics_.end() && IsMusicValid(musics_[name])) {
    PlayMusicStream(musics_[name]);
    cur_music_ = name;
  }
}

void Media::UpdateMusic() {
  if (musics_.find(cur_music_) != musics_.end())
    UpdateMusicStream(musics_[cur_music_]);
}

void Media::TogglePauseMusic() {
  if (IsMusicStreamPlaying(musics_[cur_music_]))
    PauseMusicStream(musics_[cur_music_]);
  else
    ResumeMusicStream(musics_[cur_music_]);
}

void Media::StopMusic() {
  if (IsMusicStreamPlaying(musics_[cur_music_])) {
    StopMusicStream(musics_[cur_music_]);
    cur_music_ = "";
  }
}

void Media::SaveMusicState() {
  if (IsMusicStreamPlaying(musics_[cur_music_])) {
    music_state_.push({cur_music_, GetMusicTimePlayed(musics_[cur_music_])});
  }
}

void Media::LoadMusicState() {
  if (!music_state_.empty()) {
    auto [name, time] = music_state_.top();
    music_state_.pop();
    SeekMusicStream(musics_[name], time);
    PlayMusic(name);
  }
}
