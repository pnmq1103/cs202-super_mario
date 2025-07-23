#include <iostream>
#include <memory>
#include "block.hpp"

class EmptyBlock;
class SolidBlock;
class QuestionBlock;
class MusicBlock;
class PipeBlock;
class RockBlock;

class BlockFactory {
public:
  std::unique_ptr<Block> MakeBlock(
    BlockType type, int Gid, Vector2 pos, Vector2 size, int spriteID, bool solid, bool animating);

};