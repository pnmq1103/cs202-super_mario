#include "raylib.h"
#include <string>
#include <vector>
struct SaveData {
  int highScore;
  Vector2 characterPosition;
  int mapRows;
  int mapCols;
  //store each tile x and y positions
  std::vector<std::pair<int,int>> mapTiles;
};

class FileHandler {
public:
  explicit FileHandler(int slotID);

  ~FileHandler() = default;

  bool loadGame(SaveData &data) const;

  void saveGame(SaveData &data);

private:
  std::string filename = "saved_game_slot_";
};