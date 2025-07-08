#include <cstdint>
#include <raylib.h>
#include <string>
#include <vector>

#include "../tinyfiledialogs.h"

// stuff to deal with endianness
#pragma pack(push, 1)
struct tileData {
  char tileType; // type of tile
  int x;         // x position of the tile in the map
  int y;         // y position of the tile in the map
};

struct SaveData {
  int32_t highScore;
  int32_t backgroundID;
  // character positions
  int32_t xPos;
  int32_t yPos;
  int32_t mapRows;
  int32_t mapCols;
};

#pragma pack(pop)
static_assert(
  sizeof(SaveData) == sizeof(int32_t) * 6, "SaveData has unexpected padding");
struct SaveDatawMap {
  SaveData header = {};
  // Store each tile x and y positions; must be handled separately
  std::vector<tileData> mapTiles = {};
};

class FileHandler {
private:
  std::string filename = {"saved_game_slot_"}; // intended for saving game into
                                               // slots; will look at later
public:
  explicit FileHandler(int i) {
    filename += std::to_string(i);
  }
  explicit FileHandler() = default;

  ~FileHandler() = default;

  static std::string OpenFilePath();

  std::string OpenSavePath(const std::string &defaultName = "untitled.bin");

  bool LoadFile(const std::string &path, SaveDatawMap &sd) const;

  bool SaveFile(const std::string &path, const SaveDatawMap &sd);

  // std::vector<tileData> parseMapTiles();
  // parse all tiles in map into a vector to save/load;  will look at later
};