#include "raylib.h"  
#include "src/include/tinyfiledialogs.h"
#include <cstdint>
#include <string>  
#include <vector>  

//stuff to deal with endianness
#pragma pack(push, 1)
struct tileData {
  char tileType; //type of tile 
  int x;         //x position of the tile in the map
  int y;         //y position of the tile in the map
};
struct SaveData {  
  int32_t highScore;  
  float xPos;
  float yPos;
  int32_t mapRows;  
  int32_t mapCols;  
};  
#pragma pack(pop)
static_assert(sizeof(SaveData) == sizeof(int32_t) * 3 + sizeof(float) * 2,
              "SaveData has unexpected padding");
struct SaveDatawMap {
  SaveData header;
  // Store each tile x and y positions; must be handle separately
  std::vector<tileData> mapTiles;
};

class FileHandler {  
public:  
  explicit FileHandler() = default;  

  ~FileHandler() = default;

  static std::string openFilePath();

  bool loadFile(const std::string& path, SaveDatawMap &sd) const;  

  bool saveFile(const std::string& path, const SaveDatawMap &sd);

  std::vector<tileData> parseMapTiles(); //parse all tiles in map into a vector to save/load

private:  
  std::string filename = "saved_game_slot_"; // intended for saving game into slots; will look at later  
};