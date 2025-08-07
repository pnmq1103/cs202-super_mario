#include <filesystem>
#include <string>

#include "include/core/config.hpp"
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

  static void
  SaveMapToFile(const Map &map, const std::filesystem::path &path = "");
  static void LoadMapFromFile(Map &map, const std::filesystem::path &path);

  static void SaveConfig(const Config &config);
  static void LoadConfig(Config &config);
};