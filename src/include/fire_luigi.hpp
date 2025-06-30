#pragma once
#include "include/character_state.hpp"
class FireLuigi: public CharacterState{
    private:
        Texture pre_texture_;
        Rectangle pre_frame_;
    public:
        FireLuigi(Character *Ncontext,float Nscale,bool Nleft,bool tran);
        ~FireLuigi();
        void Die();
        void Evolve();
        void Update();
        void Draw();
};