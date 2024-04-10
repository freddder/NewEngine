#include "Player.h"
#include "cPlayerCharacter.h"

namespace Player 
{
    cPlayerCharacter* playerChar;

    glm::vec3 GetPlayerPosition()
    {
        return playerChar->model->position;
    }

    glm::vec3* GetPlayerPositionRef()
    {
        return &playerChar->model->position;
    }
}
