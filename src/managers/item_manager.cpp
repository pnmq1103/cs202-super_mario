#include "include/managers/item_manager.hpp"

ItemManager::~ItemManager() {
    for (PowerUp* i : items) {
        delete i;
    }
}

void ItemManager::AddItem(PowerUp* item) {
    items.push_back(item);
}

void ItemManager::Update(float dt) {
    for (PowerUp* i : items) {
        if (i) i->Update(dt);
    }
}

void ItemManager::Render(const Texture& texture) const {
    for (PowerUp* i : items) {
        if (i) i->Render(texture);
    }
}

const std::vector<PowerUp*>& ItemManager::GetItems() const {
    return items;
}

void ItemManager::DeleteItem(int index) {
    if (index >= 0 && index < (int)items.size()) {
        delete items[index];
        items[index] = nullptr;
    }
}

void ItemManager::Clear() {
    for (PowerUp*& i : items) {
        delete i;
        i = nullptr;
    }
    items.clear();
}
