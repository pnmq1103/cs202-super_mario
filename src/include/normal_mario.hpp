#include "src/include/character_state.hpp"
class NormalMario: public CharacterState{
    public:
        NormalMario(Character *Ncontext,float Nscale);
        void update();
};