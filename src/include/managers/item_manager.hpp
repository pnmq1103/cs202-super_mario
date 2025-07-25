#pragma once
#include <vector>
#include "include/items/powerup.hpp"

class ItemManager {
private:
    std::vector<PowerUp*> items;

public:
    ~ItemManager();

    void AddItem(PowerUp* item);
    void Update(float dt);
    void Render(const Texture& texture) const;

    const std::vector<PowerUp*>& GetItems() const;
    void DeleteItem(int index);
    void Clear();
};
