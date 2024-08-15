#pragma once

#include <glm/glm.hpp>
#include <memory>

// This is where all player related info will be
// - party
// - items
// - map location

class cPlayerEntity;
class cTamedRoamingPokemon;

namespace Pokemon
{
	struct sIndividualData;
}

namespace Player
{
	extern cPlayerEntity* playerChar;
	extern std::shared_ptr<cTamedRoamingPokemon> playerPartner;

	glm::vec3 GetPlayerPosition();
	glm::vec3* GetPlayerPositionRef();

	extern Pokemon::sIndividualData partyMember1;
	extern Pokemon::sIndividualData partyMember2;
	extern Pokemon::sIndividualData partyMember3;
	extern Pokemon::sIndividualData partyMember4;
	extern Pokemon::sIndividualData partyMember5;
	extern Pokemon::sIndividualData partyMember6;

	void AddPartyMember(const Pokemon::sIndividualData& newPartyMember);
}