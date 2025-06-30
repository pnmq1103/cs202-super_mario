#pragma once
#include "include/character_state.hpp"
class NormalLuigi: public CharacterState{
    private:
        Texture pre_texture_;
        Rectangle pre_frame_;
    public:
        NormalLuigi(Character *Ncontext,float Nscale,bool Nleft, bool tran);
        ~NormalLuigi();
        void Die();
        void Update();
        void Evolve();
        void Draw();
};