#pragma once
#include "include/character_state.hpp"
class FireMario: public CharacterState{
    private:
        Texture pre_texture_;
        Rectangle pre_frame_;
    public:
        FireMario(Character *Ncontext,float Nscale,bool Nleft,bool tran);
        ~FireMario();
        void Die();
        void Evolve();
        void Update();
        void Draw();
};