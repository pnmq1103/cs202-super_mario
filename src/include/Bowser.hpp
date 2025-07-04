#pragma once
#include "Enemy.hpp"

class Bowser : public Enemy {
private:
    int hp;
    float fireTimer = 0;

public:
    Bowser(Vector2 pos, int spriteId);
    void update(float dt) override;
    void onHitFromAbove() override;
    void onHitFromSide() override;
    Enemy* clone() const override;

    int getHP() const;
};
