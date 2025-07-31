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

  static std::string OpenFile();
  static std::string SaveFile();

  static void SaveMapToFile(const Map &map);
  static void LoadMapFromFile(const std::filesystem::path &path, Map &map);
};