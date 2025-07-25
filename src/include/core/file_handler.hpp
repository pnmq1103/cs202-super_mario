#include <filesystem>
#include <string>

#include "include/core/map.hpp"

class FileHandler {
private:
  static int count;
  static const std::filesystem::path base_path;

public:
  FileHandler()  = default;
  ~FileHandler() = default;

  static std::string OpenFilePath();
  static std::string
  OpenSavePath(const std::string &default_name = "untitled.bin");

  static void SaveMapToFile(const Map &map);
  static Map LoadMapFromFile(const std::filesystem::path &path);
};