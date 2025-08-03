#include "include/enemies/piranha_plant.hpp"
#include "include/enemies/movement_strategy.hpp"

PiranhaPlant::PiranhaPlant(Vector2 pos, float Nscale)
    : Enemy(pos, Nscale, EnemyType::Piranha), is_emerging(false), emerge_timer(0.0f) {
    // Piranha plants don't move horizontally - they stay in pipes
    SetMovementStrategy(new StationaryMovement(10.0f, 1.5f)); // Bob up and down
    
    // Piranha-specific properties
    health = 1.0f;
    max_health = 1.0f;
    detection_range = 120.0f;
    
    // Piranha plants are dangerous to touch
    velocity = {0.0f, 0.0f}; // No horizontal movement
}

PiranhaPlant::~PiranhaPlant() {}

void PiranhaPlant::OnHit() {
    // Piranha plant dies when hit by projectile
    alive = false;
    state = EnemyState::Dead;
    velocity = {0.0f, 0.0f};
    
    // Death animation could show plant wilting
    std::cout << "Piranha Plant defeated! +300 points" << std::endl;
}

void PiranhaPlant::Update() {
    if (!alive) return;
    
    // Piranha plant AI - emerge when player approaches, hide when too close
    if (player_position) {
        float distance = GetDistanceToPlayer();
        
        if (distance <= 60.0f) {
            // Player too close - hide in pipe
            if (is_emerging) {
                StartHiding();
            }
        } else if (distance <= detection_range && distance > 60.0f) {
            // Player in range but not too close - emerge and attack
            if (!is_emerging) {
                StartEmerging();
            }
        } else {
            // Player far away - normal bobbing behavior
            if (is_emerging) {
                StartHiding();
            }
        }
    }
    
    // Update emergence timer
    if (is_emerging) {
        emerge_timer += GetFrameTime();
        if (emerge_timer >= 2.0f) {
            // Fully emerged, stay up for a while
            if (emerge_timer >= 4.0f) {
                StartHiding();
            }
        }
    } else {
        emerge_timer = 0.0f;
    }
    
    // Call base enemy update
    Enemy::Update();
}

void PiranhaPlant::StartEmerging() {
    is_emerging = true;
    emerge_timer = 0.0f;
    
    // Trigger emergence animation in movement strategy
    if (auto* stationary = dynamic_cast<StationaryMovement*>(GetMovementStrategy())) {
        stationary->StartEmergence();
    }
    
    state = EnemyState::Attacking; // Piranha is aggressive when emerging
    std::cout << "Piranha Plant emerging!" << std::endl;
}

void PiranhaPlant::StartHiding() {
    is_emerging = false;
    emerge_timer = 0.0f;
    state = EnemyState::Normal;
    
    std::cout << "Piranha Plant hiding!" << std::endl;
}

void PiranhaPlant::OnHitFromAbove() {
    try {
        // Piranha plants are deadly to touch from any direction
        // Mario should take damage, not the Piranha
        // This collision should be handled differently - Mario should die
        
        // However, if Mario has star power, Piranha dies
        std::cout << "Piranha Plant touched from above - Mario should take damage!" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error in PiranhaPlant::OnHitFromAbove(): " << e.what() << std::endl;
        // Fallback: mark as dead to prevent further issues
        alive = false;
        state = EnemyState::Dead;
        velocity = {0.0f, 0.0f};
    }
}

void PiranhaPlant::OnHitFromSide() {
    try {
        // Same as above - Piranha plants are dangerous to touch
        // Only projectiles or star power should damage them
        OnHit(); // For now, assume it's a projectile hit
    } catch (const std::exception& e) {
        std::cerr << "Error in PiranhaPlant::OnHitFromSide(): " << e.what() << std::endl;
        // Fallback: mark as dead
        alive = false;
        state = EnemyState::Dead;
        velocity = {0.0f, 0.0f};
    }
}

EnemyType PiranhaPlant::GetType() {
    return EnemyType::Piranha;
}

Enemy* PiranhaPlant::Clone() const {
    PiranhaPlant* clone = new PiranhaPlant(*this);
    // Clone the movement strategy
    if (movement_strategy_) {
        clone->SetMovementStrategy(new StationaryMovement(10.0f, 1.5f));
    }
    return clone;
}

bool PiranhaPlant::IsEmerging() const {
    return is_emerging;
}

void PiranhaPlant::ForceEmerge() {
    StartEmerging();
}

void PiranhaPlant::ForceHide() {
    StartHiding();
}
