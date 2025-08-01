#include <filesystem>
#include <fstream>
#include <iostream>

#include "include/core/file_handler.hpp"
#include "include/core/map.hpp"
#include "include/external/tinyfiledialogs.h"

int FileHandler::count = 0;

const std::filesystem::path FileHandler::base_path = "res/saves";

std::string FileHandler::OpenFile() {
  const char *filter[] = {"*.json"};
  const char *filename = tinyfd_openFileDialog(
    "Select file", base_path.string().c_str(), 1, filter, "(*.json)", 0);

  if (filename != nullptr)
    return filename;
  return {};
}

std::string FileHandler::SaveFile() {
  const char *filter[] = {"*.json"};
  const char *filename = tinyfd_saveFileDialog(
    "Save file as", (base_path / "untitled.json").string().c_str(), 1, filter,
    "(*.json)");

  if (filename != nullptr)
    return filename;
  return {};
}

void
FileHandler::SaveMapToFile(const Map &map, const std::filesystem::path &path) {
  std::filesystem::path full_path;
  if (path.empty() == false) {
    std::filesystem::create_directories(path.parent_path());
    full_path = path;
  } else {
    std::filesystem::create_directories(base_path);

    std::ostringstream filename;
    filename << "map_" << std::setw(2) << std::setfill('0') << count << ".json";
    ++count;

    full_path = base_path / filename.str();
  }

  std::ofstream fout(full_path);
  if (fout.is_open() == false)
    throw std::runtime_error("failed to create file");

  nlohmann::json j = map;
  fout << j.dump();
}

void FileHandler::LoadMapFromFile(Map &map, const std::filesystem::path &path) {
  if (std::filesystem::exists(path) == false)
    throw std::runtime_error("file not found");

  std::ifstream fin(path);
  if (fin.is_open() == false)
    throw std::runtime_error("cannot open file");

  nlohmann::json j;
  fin >> j;

  j.get_to(map);
}
