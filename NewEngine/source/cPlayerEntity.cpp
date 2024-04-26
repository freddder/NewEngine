#include "cPlayerEntity.h"

cPlayerEntity::cPlayerEntity(glm::vec3 pos) : cCharacterEntity(pos)
{
	spriteModel = new cPlayerSprite("Nate.png", pos);
}

cPlayerEntity::~cPlayerEntity()
{
	
}

glm::vec3 cPlayerEntity::GetModelPosition()
{
	return spriteModel->model->position;
}

glm::vec3& cPlayerEntity::GetModelPositionRef()
{
	return spriteModel->model->position;
}

void cPlayerEntity::StopMovement()
{
	dynamic_cast<cPlayerSprite*>(spriteModel)->StopMovement();
}
