#pragma once
#include <raylib.h>
#include <stdexcept>
#include <unordered_map>
#include <vector>

#include "include/core/utility.hpp"
#include "include/external/json.hpp"

struct SpriteSheet {
  int first_gidx = {0};
  int count      = {0};
  std::string texture_key;
  std::unordered_map<int, Rectangle> info;

  SpriteSheet() = default;
  SpriteSheet(int gidx, int c, std::string key, std::string path)
      : first_gidx(gidx), count(c), texture_key(key) {
    utility::ReadSpriteInfo(path, info);
  }
};

enum struct MapLayer {
  Tile1 = 0,
  Objects,
  Tile2,
  Count,
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
  void SetCell(MapLayer layer, int pos, int gidx);
  int GetCell(MapLayer layer, int pos) const;
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
        {"first_gidx", s.first_gidx},
        {"count", s.count},
        {"texture_key", s.texture_key},
        {"info", s.info}};
    }

    static void from_json(const json &j, SpriteSheet &s) {
      j.at("first_gidx").get_to(s.first_gidx);
      j.at("count").get_to(s.count);
      j.at("texture_key").get_to(s.texture_key);
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
