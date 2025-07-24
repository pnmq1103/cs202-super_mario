#pragma once
#include <vector>
#include "include/blocks/block.hpp"

class BlockManager {
private:
    std::vector<Block*> blocks;

public:
    ~BlockManager();

    void AddBlock(Block* block);
    void Update(float dt);
    void Render(const Texture& texture, const std::unordered_map<int, Rectangle>& spriteRects) const;

    const std::vector<Block*>& GetBlocks() const;
    void DeleteBlock(int index);
    void Clear();
};
