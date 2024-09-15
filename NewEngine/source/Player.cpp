#include "Player.h"
#include "cPlayerEntity.h"
#include "cTamedRoamingPokemon.h"
#include "PokemonData.h"

namespace Player 
{
    cPlayerEntity* playerChar;
    std::shared_ptr<cTamedRoamingPokemon> playerPartner;

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
    void SwitchPartyMembers(const int member1, const int member2)
    {
        if (member1 < 1 || member1 > 6 ||
            member2 < 1 || member2 > 6 ||
            member1 == member2)
            return;

        Pokemon::sIndividualData* switchMember1, switchMember2;

        //if (member1 == 1) switchMember1 = partyMember1
    }
}
