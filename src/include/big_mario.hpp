#pragma once
#include "include/character_state.hpp"
class BigMario: public CharacterState{
    private:
        Texture pre_texture_;
        Rectangle pre_frame_;
    public:
        BigMario(Character *Ncontext,float Nscale,bool Nleft,bool tran);
        ~BigMario();
        void Update();
        void Die();
        void Evolve();
        void Draw();
};