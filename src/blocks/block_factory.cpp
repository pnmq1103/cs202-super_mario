#include "include/blocks/block_factory.hpp"
#include "include/blocks/question_block.hpp"
#include "include/blocks/music_block.hpp"
#include "include/blocks/ground_block.hpp"
#include "include/blocks/empty_block.hpp"
#include "include/blocks/solid_block.hpp"
#include "include/blocks/rock_block.hpp"

std::unique_ptr<Block> BlockFactory::MakeBlock(
	BlockType type, int gid, Vector2 pos, Vector2 size, int spriteID, bool solid,
	bool animating) {
  switch (type) {
    case BlockType::Empty: {
      return std::make_unique<EmptyBlock>(
        gid, pos, size, spriteID, solid, animating);
      break;
    }
    case BlockType::Solid: {
      return std::make_unique<SolidBlock>(
        gid, pos, size, spriteID, solid, animating);
    }
    case BlockType::Question: {
      return std::make_unique<QuestionBlock>(
         gid, pos, size, spriteID, solid, animating);
    }
    case BlockType::Music: {
      return std::make_unique<MusicBlock>(
        gid, pos, size, spriteID, solid, animating);
    }
    case BlockType::Ground: {
      return std::make_unique<GroundBlock>(
        gid, pos, size, spriteID, solid, animating);
    }
    case BlockType::Rock: {
      return std::make_unique<RockBlock>(
        gid, pos, size, spriteID, spriteID, animating);
    }
  }
}