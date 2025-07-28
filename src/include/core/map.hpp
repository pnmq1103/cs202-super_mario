#pragma once
#include <raylib.h>
#include <unordered_map>
#include <vector>

#include "include/core/utility.hpp"
#include "include/external/json.hpp"

struct SpriteSheet {
  int first_gidx_ = {0};
  int count_      = {0};
  std::string texture_key_;
  std::unordered_map<int, Rectangle> info;

  SpriteSheet() = default;
  SpriteSheet(
    int first_gidx, int count, std::string texture_key, std::string path)
      : first_gidx_(first_gidx), count_(count), texture_key_(texture_key) {
    utility::ReadSpriteInfo(path, info);
  }
};

enum struct MapLayer {
  Tile1,
  Objects,
  Tile2,
};

class Map {
private:
  std::vector<SpriteSheet> sprite_sheets_;
  std::vector<std::vector<int>> layers_;

  friend struct nlohmann::adl_serializer<Map>;

public:
  Map();
  ~Map();

  void Init();
  void SetTile(int pos, int gidx);
  void SetEnemy(int pos, int gidx);
  void SetPipe(int pos, int gidx);
  int GetTile(int pos) const;
  int GetEnemy(int pos) const;
  int GetPipe(int pos) const;
  const Texture &FindTexture(std::string texture_key) const;
  const Texture &GetTexture(int gidx) const;
  Rectangle GetInfo(int gidx) const;
};

namespace nlohmann {
  template <>
  struct adl_serializer<Rectangle> {
    static void to_json(json &j, const Rectangle &r) {
      j = json{
        {"x", r.x}, {"y", r.y}, {"width", r.width}, {"height", r.height}};
    }

    static void from_json(const json &j, Rectangle &r) {
      j.at("x").get_to(r.x);
      j.at("y").get_to(r.y);
      j.at("width").get_to(r.width);
      j.at("height").get_to(r.height);
    }
  };

  template <>
  struct adl_serializer<SpriteSheet> {
    static void to_json(json &j, const SpriteSheet &s) {
      j = json{
        {"first_gidx", s.first_gidx_},
        {"count", s.count_},
        {"texture_key", s.texture_key_},
        {"info", s.info}};
    }

    static void from_json(const json &j, SpriteSheet &s) {
      j.at("first_gidx").get_to(s.first_gidx_);
      j.at("count").get_to(s.count_);
      j.at("texture_key").get_to(s.texture_key_);
      j.at("info").get_to(s.info);
    }
  };

  template <>
  struct adl_serializer<Map> {
    static void to_json(json &j, const Map &m) {
      j = json{
        {"sprite_sheets", m.sprite_sheets_},
        {"layers", m.layers_},
      };
    }

    static void from_json(const json &j, Map &m) {
      j.at("sprite_sheets").get_to(m.sprite_sheets_);
      j.at("layers").get_to(m.layers_);
    }
  };
} // namespace nlohmann
