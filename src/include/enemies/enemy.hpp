#pragma once
#include <raylib.h>
#include <unordered_map>

// Forward declaration
class MovementStrategy;

enum class EnemyType {
    Goomba,
    Koopa,
    Piranha,
    Bowser,
    Shy_Guy,
    Bullet_Bill,
    Hammer_Bro
};

enum class EnemyState {
    Normal,
    Stunned,
    Shell,
    Dead,
    Attacking,
    Hiding
};

class Enemy {
protected:
    Vector2 position;
    Vector2 velocity;
    int width, height;
    bool alive;
    int spriteId;
    EnemyType type;
    EnemyState state;
    MovementStrategy* movementStrategy_;
    
    // Enhanced properties
    float health;
    float maxHealth;
    float stunTimer;
    float invulnerabilityTimer;
    bool facingLeft;
    float detectionRange;
    
    // Interaction properties
    Vector2* playerPosition; // Reference to Mario/Luigi position
    bool isAggressive;
    float aggroRange;
    float attackCooldown;
    float attackTimer;

public:
    Enemy(Vector2 pos, int w, int h, EnemyType type, int spriteId = 0);
    virtual ~Enemy();

    // Core functionality
    virtual void Update(float dt);
    virtual void OnHitFromAbove() = 0;
    virtual void OnHitFromSide() = 0;
    
    // Enhanced interactions
    virtual void OnCharacterInteraction(Vector2 characterPos, bool characterFalling);
    virtual void OnProjectileHit(Vector2 projectilePos, int damage = 1);
    virtual void OnEnvironmentCollision(Vector2 collisionPoint);
    
    // State management
    virtual void Stun(float duration = 2.0f);
    virtual void EnterShellMode() {} // For Koopa
    virtual void ExitShellMode() {}  // For Koopa
    virtual void EnterAttackMode();
    virtual void ExitAttackMode();
    
    // Movement strategy methods
    void SetMovementStrategy(MovementStrategy* strategy);
    MovementStrategy* GetMovementStrategy() const;
    void UpdateMovement(float dt);
    
    // Physics and collision
    virtual void ApplyGravity(float deltaTime);
    virtual void HandleGroundCollision(float groundLevel = 600.0f);
    virtual void HandleWallCollision();
    void ReverseDirection();
    
    // Character interaction system
    void SetPlayerReference(Vector2* playerPos) { playerPosition = playerPos; }
    bool IsPlayerInRange(float range) const;
    Vector2 GetDirectionToPlayer() const;
    float GetDistanceToPlayer() const;
    
    // Getters and setters
    Rectangle GetRect() const;
    Vector2 GetPosition() const;
    Vector2 GetVelocity() const;
    EnemyType GetType() const;
    EnemyState GetState() const;
    bool IsAlive() const;
    float GetHealth() const { return health; }
    float GetMaxHealth() const { return maxHealth; }
    bool IsStunned() const { return state == EnemyState::Stunned; }
    bool IsInvulnerable() const { return invulnerabilityTimer > 0; }
    bool IsFacingLeft() const { return facingLeft; }
    
    void SetPosition(Vector2 pos);
    void SetVelocity(Vector2 vel);
    void SetAlive(bool status);
    void SetState(EnemyState newState);
    void SetFacing(bool left) { facingLeft = left; }
    
    // Utility methods
    bool CheckCollision(Rectangle other) const;
    virtual Enemy* Clone() const = 0;
    virtual void Render(Texture &tex, const std::unordered_map<int, Rectangle> &spriteRects) const;
    
    // Advanced behaviors
    virtual bool CanJump() const { return false; }
    virtual void Jump(float force = 100.0f) { (void)force; } // Fix unused parameter warning
    virtual bool CanAttack() const { return false; }
    virtual void PerformAttack() {}
    virtual void OnPlayerDetected() {} // Made virtual with empty implementation
    virtual void OnPlayerLost() {} // Made virtual with empty implementation
    
protected:
    // Helper methods for derived classes
    void UpdateTimers(float deltaTime);
    void UpdatePlayerDetection();
    void DealDamage(float damage);
    bool ShouldReverseDirection() const;
};
