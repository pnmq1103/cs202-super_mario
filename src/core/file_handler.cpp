#include <fstream>
#include <iostream>

#include "include/core/file_handler.hpp"
#include "include/external/json.hpp"

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

// bool FileHandler::SaveFile(const std::string &path, const SaveData &sd) {
//   json j;
//   j["Score"]           = sd.score;
//   j["Lives"]           = sd.lives;
//   j["Background ID"]   = sd.backgroundID;
//   j["Game time"]       = sd.gameTime;
//   j["Character Pos X"] = sd.charPosX;
//   j["Character Pos Y"] = sd.charPosY;
//
//   j["Map tiles"] = json::array();
//   for (auto &tile : sd.mapTiles) {
//     j["Map tiles"].push_back(
//       {{"type", std::string(1, tile.tileType)}, {"x", tile.x}, {"y",
//       tile.y}});
//   }
//
//   std::ofstream out(path);
//   if (!out.is_open())
//     return false;
//   out << j.dump(2); // pretty-print with 2-space indent
//   return out.good();
// }
//
// bool FileHandler::LoadFile(const std::string &path, SaveData &sd) const {
//   std::ifstream in(path);
//   if (!in.is_open())
//     return false;
//
//   json j;
//   try {
//     in >> j; // throws on parse error
//   } catch (const json::parse_error &e) {
//     std::cerr << "JSON parse error at byte " << e.byte << ": " << e.what()
//               << "\n";
//     return false;
//   }
//
//   // 1) Read back your scalar fields
//   sd.score        = j.value("Score", sd.score);
//   sd.lives        = j.value("Lives", sd.lives);
//   sd.backgroundID = j.value("Background ID", sd.backgroundID);
//   sd.gameTime     = j.value("Game time", sd.gameTime);
//   sd.charPosX     = j.value("Character Pos X", sd.charPosX);
//   sd.charPosY     = j.value("Character Pos Y", sd.charPosY);
//
//   // 2) Read back the tile array
//   sd.mapTiles.clear();
//   auto &tiles = j.at("Map tiles");
//   sd.mapTiles.reserve(tiles.size());
//   for (auto &jtile : tiles) {
//     tileData t;
//     auto s     = jtile.at("type").get<std::string>();
//     t.tileType = s.empty() ? '\0' : s[0];
//     t.x        = jtile.at("x").get<int>();
//     t.y        = jtile.at("y").get<int>();
//     sd.mapTiles.push_back(t);
//   }
//
//   return true;
// }
