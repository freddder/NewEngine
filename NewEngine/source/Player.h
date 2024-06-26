#pragma once

#include <glm/glm.hpp>

// This is where all player related info will be
// - party
// - items
// - map location

class cPlayerEntity;

namespace Pokemon
{
	struct sPokemonData;
}

namespace Player
{
	extern cPlayerEntity* playerChar;

	glm::vec3 GetPlayerPosition();
	glm::vec3* GetPlayerPositionRef();

	extern Pokemon::sPokemonData partyMember1;
	extern Pokemon::sPokemonData partyMember2;
	extern Pokemon::sPokemonData partyMember3;
	extern Pokemon::sPokemonData partyMember4;
	extern Pokemon::sPokemonData partyMember5;
	extern Pokemon::sPokemonData partyMember6;

	void AddPartyMember(const Pokemon::sPokemonData& newPartyMember);
}