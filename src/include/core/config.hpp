#pragma once
#include "include/external/json.hpp"

class Config {
private:
  float music_volume_ = {1};
  float sound_volume_ = {1};

  friend struct nlohmann::adl_serializer<Config>;

public:
  ~Config() = default;
  Config()  = default;

  void Init();
  void Reset();

  float GetMusicVolume();
  float GetSoundVolume();
  void SetMusicVolume(float volume);
  void SetSoundVolume(float volume);
};

namespace nlohmann {
  template <>
  struct adl_serializer<Config> {
    static void to_json(json &j, const Config &c) {
      j = json{
        {"music_volume", c.music_volume_},
        {"sound_volume", c.sound_volume_},
      };
    }

    static void from_json(const json &j, Config &c) {
      j.at("music_volume").get_to(c.music_volume_);
      j.at("sound_volume").get_to(c.sound_volume_);
    }
  };
} // namespace nlohmann