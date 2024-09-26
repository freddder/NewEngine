#include "cPlayerEntity.h"

cPlayerEntity::cPlayerEntity() : cCharacterEntity(glm::vec3(0.f))
{
	spriteModel = new cPlayerSprite("Nate.png", glm::vec3(0.f));
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
