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

class Map {
private:
  int block_width_  = {50};
  int block_height_ = {25};
  std::vector<SpriteSheet> sprite_sheets_;

  std::vector<int> tile_layer_;
  std::vector<int> enemy_layer_;

  friend struct nlohmann::adl_serializer<Map>;

public:
  Map(int block_width, int block_height);
  ~Map();

  void Init();
  void SetTile(int pos, int gidx);
  void SetEnemy(int pos, int gidx);
  int GetTile(int pos) const;
  int GetEnemy(int pos) const;
  const Texture &FindTexture(std::string texture_key) const;
  const Texture &GetTexture(int gidx) const;
  Rectangle GetInfo(int gidx);
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
        {"block_width", m.block_width_},
        {"block_height", m.block_height_},
        {"sprite_sheets", m.sprite_sheets_},
        {"tile_layer", m.tile_layer_},
        {"enemy_layer", m.enemy_layer_}};
    }

    static void from_json(const json &j, Map &m) {
      j.at("block_width").get_to(m.block_width_);
      j.at("block_height").get_to(m.block_height_);
      j.at("sprite_sheets").get_to(m.sprite_sheets_);
      j.at("tile_layer").get_to(m.tile_layer_);
      j.at("enemy_layer").get_to(m.enemy_layer_);
    }
  };
} // namespace nlohmann
