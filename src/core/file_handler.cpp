#include <filesystem>
#include <fstream>
#include <iostream>

#include "include/core/file_handler.hpp"
#include "include/core/map.hpp"
#include "include/external/tinyfiledialogs.h"

int FileHandler::count = 0;

const std::filesystem::path FileHandler::base_path = "res/saves";

std::string FileHandler::OpenFilePath() {
  const char *filter[] = {"*.json"};
  const char *fn
    = tinyfd_openFileDialog("Select file", "", 1, filter, "(*.json)", 0);

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

void FileHandler::SaveMapToFile(const Map &map) {
  std::filesystem::create_directories(base_path);

  std::ostringstream filename;
  filename << "map_" << std::setw(2) << std::setfill('0') << count << ".json";
  ++count;

  std::filesystem::path full_path = base_path / filename.str();

  std::ofstream fout(full_path);
  if (!fout.is_open())
    throw std::runtime_error("failed to create file");

  nlohmann::json j = map;
  fout << j.dump();
}

Map LoadMapFromFile(const std::filesystem::path &path) {
  if (std::filesystem::exists(path) == false)
    throw std::runtime_error("file not found");

  std::ifstream fin(path);
  if (fin.is_open() == false)
    throw std::runtime_error("cannot open file");

  nlohmann::json j;
  fin >> j;

  Map map(0, 0);
  j.get_to(map);

  return map;
}
