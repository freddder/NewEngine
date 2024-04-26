#pragma once

#include <glm/glm.hpp>

// This is where all player related info will be
// - party
// - items
// - map location

class cPlayerEntity;

namespace Player
{
	extern cPlayerEntity* playerChar;

	glm::vec3 GetPlayerPosition();
	glm::vec3* GetPlayerPositionRef();
}