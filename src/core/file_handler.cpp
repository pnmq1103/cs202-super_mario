#include <cstdint>
#include <fstream>
#include <string>

#include "include/core/file_handler.hpp"

// helper functions for endianness handling
//   Helpers: convert to/from little endian
uint32_t toLE(uint32_t v) {
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
  return v;
#else
  return __builtin_bswap32(v);
#endif
}

uint32_t fromLE(uint32_t v) {
  return toLE(v); // same op
}

std::string FileHandler::OpenFilePath() {
  const char *filter[] = {"*.bin"};
  if (
    const char *fn = tinyfd_openFileDialog(
      "Select data file", "", 1, filter, "Binary Files (*.bin)", 0)) {
    return fn;
  }
  return {};
}

std::string FileHandler::OpenSavePath(const std::string &defaultName) {
  const char *filter[] = {"*.bin"};
  const char *fn       = tinyfd_saveFileDialog(
    "Save data file as…", defaultName.c_str(), 1, filter,
    "Binary Files (*.bin)");
  return fn ? std::string(fn) : std::string{};
}

bool FileHandler::SaveFile(const std::string &path, const SaveDatawMap &sd) {
  std::ofstream out(path, std::ios::binary);
  if (!out)
    return false;

  auto w32 = [&](uint32_t v) {
    v = toLE(v);
    out.write(reinterpret_cast<const char *>(&v), sizeof(v));
  };

  // write the rest
  w32(static_cast<uint32_t>(sd.header.highScore));
  w32(static_cast<uint32_t>(sd.header.backgroundID));
  w32(static_cast<uint32_t>(sd.header.xPos));
  w32(static_cast<uint32_t>(sd.header.yPos));
  w32(static_cast<uint32_t>(sd.header.mapCols));
  w32(static_cast<uint32_t>(sd.header.mapRows));

  // write tile count and map tiles
  uint32_t count = static_cast<uint32_t>(sd.mapTiles.size());
  w32(count);
  if (count > 0) {
    out.write(
      reinterpret_cast<const char *>(sd.mapTiles.data()),
      count * sizeof(tileData));
  }

  return out.good();
}

bool FileHandler::LoadFile(const std::string &path, SaveDatawMap &sd) const {
  std::ifstream in(path, std::ios::binary);
  if (!in)
    return false;

  auto r32 = [&]() {
    uint32_t v;
    in.read(reinterpret_cast<char *>(&v), sizeof(v));
    return fromLE(v);
  };

  // the rest of the stuff
  sd.header.highScore    = static_cast<int32_t>(r32());
  sd.header.backgroundID = static_cast<int32_t>(r32());
  sd.header.xPos         = static_cast<int32_t>(r32());
  sd.header.yPos         = static_cast<int32_t>(r32());
  sd.header.mapCols      = static_cast<int32_t>(r32());
  sd.header.mapRows      = static_cast<int32_t>(r32());

  // map tiles
  uint32_t count = r32();
  sd.mapTiles.resize(count);
  if (count > 0) {
    in.read(
      reinterpret_cast<char *>(sd.mapTiles.data()), count * sizeof(tileData));
  }

  return in.good();
}
