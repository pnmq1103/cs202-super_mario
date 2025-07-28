//#include "include/managers/block_manager.hpp"
//
//BlockManager::~BlockManager() {
//    for (Block* b : blocks) {
//        delete b;
//    }
//}
//
//void BlockManager::AddBlock(Block* block) {
//    blocks.push_back(block);
//}
//
//void BlockManager::Update(float dt) {
//    for (Block* b : blocks) {
//        if (b) b->Update(dt);
//    }
//}
//
//void BlockManager::Render(const Texture& texture, const std::unordered_map<int, Rectangle>& spriteRects) const {
//    for (Block* b : blocks) {
//        if (b) b->Render(texture, spriteRects);
//    }
//}
//
//const std::vector<Block*>& BlockManager::GetBlocks() const {
//    return blocks;
//}
//
//void BlockManager::DeleteBlock(int index) {
//    if (index >= 0 && index < (int)blocks.size()) {
//        delete blocks[index]; 
//        blocks[index] = nullptr; 
//    }
//}
//
//void BlockManager::Clear() {
//    for (Block*& b : blocks) {
//        delete b;
//        b = nullptr;
//    }
//    blocks.clear();
//}
