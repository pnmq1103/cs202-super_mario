#include "include/characters/projectile_pool.hpp"

ProjectilePool::ProjectilePool(CollisionHandler *Ncollision)
    : projectile_list_(7, nullptr) {
  collision_ = Ncollision;
  for (int i = 0; i < 3; ++i) {
    projectile_list_[i]     = new MarioFireball();
    projectile_list_[i + 3] = new EnemyFireball();
    collision_->AddProjectile(projectile_list_[i]);
    collision_->AddProjectile(projectile_list_[i + 3]);
  }
  projectile_list_[6] = new ElectricBall();
  collision_->AddProjectile(projectile_list_[6]);
}
ProjectilePool::~ProjectilePool() {
  for (int i = 0; i < 7; ++i)
    delete projectile_list_[i];
}
void ProjectilePool::ShootMarioFireball(Vector2 Nposition, bool to_left) {
  for (int i = 0; i < 3; ++i) {
    if (projectile_list_[i]->IsDestroyed()) {
      projectile_list_[i]->Renew(Nposition, to_left);
      break;
    }
  }
}
void ProjectilePool::ShootEnemyFireball(Vector2 Nposition, bool to_left) {
  for (int i = 3; i < 6; ++i) {
    if (projectile_list_[i]->IsDestroyed()) {
      projectile_list_[i]->Renew(Nposition, to_left);
      return;
    }
  }
}
void ProjectilePool::ShootElectricBall(Vector2 Nposition, bool to_left) {
  if (projectile_list_[6]->IsDestroyed())
    projectile_list_[6]->Renew(Nposition, to_left);
}
void ProjectilePool::SetFrameCount() {
  for (int i = 0; i < 7; ++i)
    projectile_list_[i]->SetFrameCount();
}
void ProjectilePool::Update() {
  for (int i = 0; i < 7; ++i)
    projectile_list_[i]->Update();
}
void ProjectilePool::Draw() {
  for (int i = 0; i < 7; ++i)
    projectile_list_[i]->Draw();
}

bool ProjectilePool::HasActiveMarioFireball() const {
  for (int i = 0; i < 3; ++i) {
    if (!projectile_list_[i]->IsDestroyed()) {
      return true;
    }
  }
  return false;
}

bool ProjectilePool::HasActiveElectricBall() const {
  return !projectile_list_[6]->IsDestroyed();
}