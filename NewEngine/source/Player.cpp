#include "Player.h"
#include "cPlayerEntity.h"

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
}
