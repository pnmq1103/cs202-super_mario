#include <fstream>
#include <iostream>

#include "include/core/file_handler.hpp"
#include "include/external/json.hpp"
#include "include/external/tinyfiledialogs.h"

using json = nlohmann::json;

std::string FileHandler::OpenFilePath() {
  const char *filter[] = {"*.json"};
  const char *fn
    = tinyfd_openFileDialog("Select file", "", 1, filter, "JSON", 0);

  if (fn != nullptr)
    return fn;
  return {};
}

std::string FileHandler::OpenSavePath(const std::string &default_name) {
  const char *filter[] = {"*.json"};
  const char *fn       = tinyfd_saveFileDialog(
    "Save file as", default_name.c_str(), 1, filter, "(*.json)");

  if (fn != nullptr)
    return fn;
  return {};
}

void FileHandler::ReadSpriteInfo(
  const std::string &path, std::unordered_map<int, Rectangle> &sprites) {
  std::ifstream fin;
  fin.open(path);
  if (fin.is_open()) {
    int id;
    float x, y, w, h;
    while (fin >> id) {
      if (fin >> x >> y >> w >> h)
        sprites[id] = {x, y, w, h};
      else
        throw std::runtime_error("malformed input");
    }
  } else
    throw std::runtime_error("invalid file");
}
/* bool FileHandler::SaveFile(const std::string &path, const SaveData &sd) {
  json j;
  j["Score"]              = sd.score;
  j["Lives"]              = sd.lives;
  j["Background ID"]      = sd.backgroundID;
  j["Game time"]          = sd.gameTime;
  j["Character Position"] = {{"x", sd.charPosition.x}, {"y",
sd.charPosition.y}};

  j["Map tiles"] = json::array();
  for (auto &tile : sd.mapTiles) {
    json tileObj = {
      {"type", tile.tileType},
      {"position", {tile.position.x, tile.position.y}},
      {"spriteID", tile.spriteID}};
    j["Map tiles"].push_back(tileObj);
  }

  std::ofstream out(path);
  if (!out.is_open())
    return false;
  out << j.dump(2);
  return out.good();
}

bool FileHandler::LoadFile(const std::string &path, SaveData &sd) const {
  std::ifstream in(path);
  if (!in.is_open())
    return false;

  json j;
  try {
    in >> j; // throws on parse error
  } catch (const json::parse_error &e) {
    std::cerr << "JSON parse error at byte " << e.byte << ": " << e.what()
              << "\n";
    return false;
  }

  sd.score        = j["Score"];
  sd.lives        = j["Lives"];
  sd.backgroundID = j["Background ID"];
  sd.gameTime     = j["Game time"];
  sd.charPosition.x = j["Character position]"]["x"].get<float>();
  sd.charPosition.y = j["Character position]"]["y"].get<float>();


  sd.mapTiles.clear();
  auto &tiles = j.at("Map tiles");
  sd.mapTiles.reserve(tiles.size());
  for (auto &jtile : tiles) {
    tileData t;
    auto s     = jtile.at("type").get<std::string>();
    t.tileType = s.empty() ? '\0' : s[0];
    t.position.x        = jtile.at("x").get<float>();
    t.position.y      = jtile.at("y").get<float>();
    sd.mapTiles.push_back(t);
  }

  return true;
}
*/
