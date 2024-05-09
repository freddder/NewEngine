#include "Player.h"
#include "cPlayerEntity.h"
#include "PokemonData.h"

namespace Player 
{
    cPlayerEntity* playerChar;

    Pokemon::sPokemonData partyMember1;
    Pokemon::sPokemonData partyMember2;
    Pokemon::sPokemonData partyMember3;
    Pokemon::sPokemonData partyMember4;
    Pokemon::sPokemonData partyMember5;
    Pokemon::sPokemonData partyMember6;

    glm::vec3 GetPlayerPosition()
    {
        return playerChar->GetModelPosition();
    }

    glm::vec3* GetPlayerPositionRef()
    {
        return &playerChar->GetModelPositionRef();
    }
}
