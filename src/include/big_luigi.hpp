#pragma once
#include "include/character_state.hpp"
class BigLuigi: public CharacterState{
    private:
        Texture pre_texture_;
        Rectangle pre_frame_;
    public:
        BigLuigi(Character *Ncontext,float Nscale,bool Nleft,bool tran);
        ~BigLuigi();
        void Update();
        void Die();
        void Evolve();
        void Draw();
};