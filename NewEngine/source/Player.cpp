#include "Player.h"
#include "cPlayerEntity.h"
#include "PokemonData.h"

namespace Player 
{
    cPlayerEntity* playerChar;

    glm::vec3 GetPlayerPosition()
    {
        return playerChar->GetModelPosition();
    }

    glm::vec3* GetPlayerPositionRef()
    {
        return &playerChar->GetModelPositionRef();
    }

    Pokemon::sPokemonData partyMember1;
    Pokemon::sPokemonData partyMember2;
    Pokemon::sPokemonData partyMember3;
    Pokemon::sPokemonData partyMember4;
    Pokemon::sPokemonData partyMember5;
    Pokemon::sPokemonData partyMember6;

    void AddPartyMember(const Pokemon::sPokemonData& newPartyMember)
    {
        if (partyMember1.nationalDexNumber == 0) partyMember1 = newPartyMember;
        else if (partyMember2.nationalDexNumber == 0) partyMember2 = newPartyMember;
        else if (partyMember3.nationalDexNumber == 0) partyMember3 = newPartyMember;
        else if (partyMember4.nationalDexNumber == 0) partyMember4 = newPartyMember;
        else if (partyMember5.nationalDexNumber == 0) partyMember5 = newPartyMember;
        else if (partyMember6.nationalDexNumber == 0) partyMember6 = newPartyMember;
    }
}
