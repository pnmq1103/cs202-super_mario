#pragma once
#include "Enemy.hpp"

class PiranhaPlant : public Enemy {
private:
    float time = 0;
    float interval = 2.0f;
    bool up = false;

public:
    PiranhaPlant(Vector2 pos, int spriteId);
    void update(float dt) override;
    void onHitFromAbove() override;
    void onHitFromSide() override;
    Enemy* clone() const override;
};
