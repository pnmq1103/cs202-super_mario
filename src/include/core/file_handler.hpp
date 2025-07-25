#include <cstdint>
#include <raylib.h>
#include <string>
#include <unordered_map>
#include <vector>

struct tileData {
  std::string tileType;
  Vector2 position;
  int spriteID;
};

struct SaveData {
  int lives;
  int score;
  float gameTime;
  int backgroundID;
  Vector2 charPosition;
  std::vector<tileData> mapTiles = {};
};

class FileHandler {
private:
  // intended for saving game into slots; will look at later
  std::string filename = {"saved_game_slot_"};

public:
  explicit FileHandler(int i) {
    filename += std::to_string(i);
  }
  explicit FileHandler() = default;

  ~FileHandler() = default;

  static std::string OpenFilePath();

  std::string OpenSavePath(const std::string &default_name = "untitled.bin");

  static void ReadSpriteInfo(
    const std::string &path, std::unordered_map<int, Rectangle> &sprites);

  // bool LoadFile(const std::string &path, SaveData &sd) const;
  // bool SaveFile(const std::string &path, const SaveData &sd);

  // std::vector<tileData> parseMapTiles();
  // parse all tiles in map into a vector to save/load;  will look at later
};