#include "FileHandler.hpp"
#include <fstream>
#include <string>

FileHandler::FileHandler(int slotID) {
  filename += std::to_string(slotID);
}

bool FileHandler::loadGame(SaveData &data) const {
  std::ifstream in(filename, std::ios::binary);
  if (!in.is_open())
    return false;
  in.read(reinterpret_cast<char *>(&data.highScore), sizeof(data.highScore));
  in.read(reinterpret_cast<char *>(&data.characterPosition.x),
          sizeof(data.characterPosition.x));
  in.read(reinterpret_cast<char *>(&data.characterPosition.y),
          sizeof(data.characterPosition.y));
  in.read(reinterpret_cast<char *>(&data.mapCols), sizeof(data.mapCols));
  in.read(reinterpret_cast<char *>(&data.mapRows), sizeof(data.mapRows));
  long long tilesNum = data.mapTiles.size();
  in.read(reinterpret_cast<char *>(&tilesNum), sizeof(tilesNum));
  if (tilesNum > 0)
    in.read(reinterpret_cast<char *>(&data.mapTiles), sizeof(data.mapTiles) * tilesNum);
  return in.good();
}

void FileHandler::saveGame(SaveData& data) {
  std::ofstream out(filename, std::ios::binary);
  if (!out.is_open())
    return;
  out.write(reinterpret_cast<char *>(&data.highScore), sizeof(data.highScore));
  out.write(reinterpret_cast<char *>(&data.characterPosition.x),
          sizeof(data.characterPosition.x));
  out.write(reinterpret_cast<char *>(&data.characterPosition.y),
          sizeof(data.characterPosition.y));
  out.write(reinterpret_cast<char *>(&data.mapCols), sizeof(data.mapCols));
  out.write(reinterpret_cast<char *>(&data.mapRows), sizeof(data.mapRows));
  long long tilesNum = data.mapTiles.size();
  out.write(reinterpret_cast<char *>(&tilesNum), sizeof(tilesNum));
  if (tilesNum > 0)
    out.write(reinterpret_cast<char *>(&data.mapTiles),
            sizeof(data.mapTiles) * tilesNum);
}

