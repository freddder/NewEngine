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

    Pokemon::sIndividualData partyMember1;
    Pokemon::sIndividualData partyMember2;
    Pokemon::sIndividualData partyMember3;
    Pokemon::sIndividualData partyMember4;
    Pokemon::sIndividualData partyMember5;
    Pokemon::sIndividualData partyMember6;

    void AddPartyMember(const Pokemon::sIndividualData& newPartyMember)
    {
        if (partyMember1.nationalDexNumber == 0) partyMember1 = newPartyMember;
        else if (partyMember2.nationalDexNumber == 0) partyMember2 = newPartyMember;
        else if (partyMember3.nationalDexNumber == 0) partyMember3 = newPartyMember;
        else if (partyMember4.nationalDexNumber == 0) partyMember4 = newPartyMember;
        else if (partyMember5.nationalDexNumber == 0) partyMember5 = newPartyMember;
        else if (partyMember6.nationalDexNumber == 0) partyMember6 = newPartyMember;
    }
}
